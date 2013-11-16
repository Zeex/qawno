#include <QDebug>
#include <QPainter>
#include <QSettings>
#include <QTextEdit>
#include <QTextBlock>
#include <QTextCursor>

#include "EditorWidget.h"
#include "SyntaxHighlighter.h"

EditorLineNumberArea::EditorLineNumberArea(EditorWidget *editor)
  : QWidget(editor)
{
  connect(editor, SIGNAL(updateRequest(QRect, int)),
          this, SLOT(update(QRect, int)));
  connect(editor, SIGNAL(blockCountChanged(int)),
          this, SLOT(updateWidth(int)));
  connect(editor, SIGNAL(blockCountChanged(int)),
          this, SLOT(updateGeometry()));
}

EditorLineNumberArea::~EditorLineNumberArea() {
  // nothing
}

EditorWidget *EditorLineNumberArea::editor() const {
  return static_cast<EditorWidget*>(parent());
}

QSize EditorLineNumberArea::sizeHint() const {
  int lineCount = editor()->blockCount();
  int digitWidth = fontMetrics().width(QLatin1Char('0'));
  int numDigits = QString::number(lineCount).length();
  int width =  digitWidth * numDigits * 2;
  return QSize(width, 0);
}

void EditorLineNumberArea::paintEvent(QPaintEvent *event) {
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
      painter.drawText(width() * 0.25, top, width() * 0.5,
                       fontMetrics().height(), Qt::AlignRight, lineNumber);
    }
    block = block.next();
    top = bottom;
    bottom = top + editor()->blockBoundingRect(block).height();
  }
  while (block.isValid() && block.isVisible() &&
         top <= event->rect().bottom());
}

void EditorLineNumberArea::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event);
  updateGeometry();
}

void EditorLineNumberArea::update(const QRect &rect, int dy) {
  if (dy != 0) {
    scroll(0, dy);
  } else {
    QWidget::update(0, rect.y(), sizeHint().width(), rect.height());
  }
  updateWidth();
}

void EditorLineNumberArea::updateWidth(int) {
  editor()->setViewportMargins(sizeHint().width(), 0, 0, 0);
}

void EditorLineNumberArea::updateGeometry() {
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
    tabStop_(0),
    lineNumberArea_(this),
    highlighter_(this)
{
  QSettings settings;

  QFont font = defaultFont();
  font.fromString(settings.value("Font/Editor", font).toString());
  setFont(font);

  setTabStop(settings.value("Editor/TabStop", 4).toInt());

  setLineWrapMode(NoWrap);
  setUndoRedoEnabled(true);

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
  settings.setValue("Font/Editor", font().toString());
  settings.setValue("Editor/TabStop", tabStop());
}

void EditorWidget::jumpToLine(long line) {
  if (line > 0 && line <= blockCount()) {
    QTextCursor cursor = textCursor();
    int pos = document()->findBlockByLineNumber(line - 1).position();
    cursor.setPosition(pos);
    setTextCursor(cursor);
  }
}

int EditorWidget::tabStop() const {
  return tabStop_;
}

void EditorWidget::setTabStop(int chars) {
  tabStop_ = chars;
  setTabStopWidth(fontMetrics().width(' ') * chars);
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
