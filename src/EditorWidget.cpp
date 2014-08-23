#include <QPainter>
#include <QSettings>
#include <QTextEdit>
#include <QTextBlock>
#include <QTextCursor>

#include "EditorWidget.h"
#include "SyntaxHighlighter.h"

EditorLineNumberWidget::EditorLineNumberWidget(EditorWidget *editor)
  : QWidget(editor)
{
  connect(editor, SIGNAL(updateRequest(QRect, int)),
          this, SLOT(update(QRect, int)));
  connect(editor, SIGNAL(blockCountChanged(int)),
          this, SLOT(updateWidth(int)));
  connect(editor, SIGNAL(blockCountChanged(int)),
          this, SLOT(updateGeometry()));
}

EditorLineNumberWidget::~EditorLineNumberWidget() {
  // nothing
}

EditorWidget *EditorLineNumberWidget::editor() const {
  return static_cast<EditorWidget*>(parent());
}

QSize EditorLineNumberWidget::sizeHint() const {
  int lineCount = editor()->blockCount();
  int digitWidth = fontMetrics().width(QLatin1Char('0'));
  int numDigits = QString::number(lineCount).length();
  int width =  digitWidth * (numDigits + 2);
  return QSize(width, 0);
}

void EditorLineNumberWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setPen(palette().foreground().color());
  painter.fillRect(event->rect(), palette().background().color());

  QTextBlock block = editor()->firstVisibleBlock();
  QPointF contentOffset = editor()->contentOffset();
  QRectF boundingGeometry = editor()->blockBoundingGeometry(block);

  qreal top = boundingGeometry.translated(contentOffset).top();
  qreal bottom = top + editor()->blockBoundingRect(block).height();

  do {
    if (bottom >= event->rect().top()) {
      QString lineNumber = QString::number(block.blockNumber() + 1);
      int digitWidth = fontMetrics().width(QLatin1Char('0'));
      int numDigits = lineNumber.length();
      QRect rect(digitWidth, top, digitWidth * numDigits, bottom);
      painter.drawText(rect, Qt::AlignRight, lineNumber);
    }
    block = block.next();
    top = bottom;
    bottom = top + editor()->blockBoundingRect(block).height();
  }
  while (block.isValid() && block.isVisible()
         && top <= event->rect().bottom());
}

void EditorLineNumberWidget::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event);
  updateGeometry();
  updateWidth();
}

void EditorLineNumberWidget::update(const QRect &rect, int dy) {
  if (dy != 0) {
    scroll(0, dy);
  } else {
    QWidget::update(0, rect.y(), sizeHint().width(), rect.height());
  }
}

void EditorLineNumberWidget::updateWidth(int) {
  editor()->setViewportMargins(sizeHint().width(), 0, 0, 0);
}

void EditorLineNumberWidget::updateGeometry() {
  QRect cr = editor()->contentsRect();
  setGeometry(cr.left(), cr.top(), sizeHint().width(), cr.height());
}

static QFont defaultFont() {
  #ifdef Q_OS_WIN32
    QFont font("Courier New");
  #else
    QFont font("Monospace");
  #endif
  font.setStyleHint(QFont::Monospace);
  return font;
}

EditorWidget::EditorWidget(QWidget *parent)
  : QPlainTextEdit(parent),
    lineNumberArea_(this),
    highlighter_(this)
{
  QSettings settings;

  QFont font = defaultFont();
  font.fromString(settings.value("EditorFont", font).toString());
  setFont(font);

  int indentChar = settings.value("IndentChar", IndentTab).toInt();
  setIndentChar(static_cast<IndentChar>(indentChar));
  setIndentSize(settings.value("IndentSize", 4).toInt());
  setTabSize(settings.value("TabSize", 4).toInt());

  setLineWrapMode(NoWrap);
  setUndoRedoEnabled(true);
  setCursorWidth(2);

  QPalette palette;
  palette.setColor(lineNumberArea_.backgroundRole(), Qt::lightGray);
  palette.setColor(lineNumberArea_.foregroundRole(), Qt::black);
  lineNumberArea_.setPalette(palette);

  highlighter_.setDocument(document());

  connect(this, SIGNAL(cursorPositionChanged()), SLOT(highlightCurrentLine()));
  highlightCurrentLine();
}

EditorWidget::~EditorWidget() {
  QSettings settings;
  settings.setValue("EditorFont", font().toString());
  settings.setValue("IndentChar", indentChar());
  settings.setValue("IndentSize", indentSize());
  settings.setValue("TabSize", tabSize());
}

void EditorWidget::jumpToLine(long line) {
  if (line > 0 && line <= blockCount()) {
    QTextCursor cursor = textCursor();
    int pos = document()->findBlockByLineNumber(line - 1).position();
    cursor.setPosition(pos);
    setTextCursor(cursor);
  }
}

void EditorWidget::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event);
  // FIXME: This should be done somwhere inside EditorLineNumberWidget.
  lineNumberArea_.updateGeometry();
}

void EditorWidget::keyPressEvent(QKeyEvent *event) {
  QTextCursor cursor = textCursor();
  if (cursor.hasSelection()) {
    switch (event->key()) {
      case Qt::Key_Tab:
        indentSelection(cursor);
        event->accept();
        return;
      case Qt::Key_Backtab:
        unindentSelection(cursor);
        event->accept();
        return;
    }
  }
  QPlainTextEdit::keyPressEvent(event);
}

void EditorWidget::highlightCurrentLine() {
  if (!isReadOnly()) {
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(Qt::lightGray).lighter(120));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    setExtraSelections(extraSelections << selection);
  }
}

void EditorWidget::editSelection(QTextCursor cursor,
                             std::function<void(QTextCursor cursor)> callback) {
  int start = cursor.selectionStart();
  int end = cursor.selectionEnd();

  cursor.setPosition(start);
  int startBlock = cursor.blockNumber();

  cursor.setPosition(end);
  int endBlock = cursor.blockNumber();

  cursor.setPosition(start);
  cursor.beginEditBlock();

  for (int i = 0; i <= endBlock - startBlock; i++) {
    cursor.movePosition(QTextCursor::StartOfBlock);
    callback(cursor);
    cursor.movePosition(QTextCursor::NextBlock);
  }

  cursor.endEditBlock();
}

void EditorWidget::indentSelection(QTextCursor cursor) {
  editSelection(cursor, [this](QTextCursor cursor) {
    switch (indentChar_) {
      case IndentTab:
        cursor.insertText("\t");
        break;
      case IndentSpace:
        for (int i = 0; i < indentSize_; i++) {
          cursor.insertText(" ");
        }
        break;
    }
  });
}

void EditorWidget::unindentSelection(QTextCursor cursor) {
  editSelection(cursor, [this](QTextCursor cursor) {
    int removedChars = 0;
    int blockStart = cursor.position();
    do {
      cursor.setPosition(blockStart + 1, QTextCursor::KeepAnchor);
      QString text = cursor.selectedText();
      if (text == "\t") {
        cursor.removeSelectedText();
        removedChars += tabSize_;
      } else if (text == " ") {
        cursor.removeSelectedText();
        removedChars++;
      } else {
        break;
      }
    } while (removedChars < indentSize_);
  });
}
