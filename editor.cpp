#include <QtGui>

#include "editor.h"
#include "highlighter.h"

LineNumberArea::LineNumberArea(Editor *edit)
	: QWidget(edit)
	, m_editor(edit)
{
}

QSize LineNumberArea::sizeHint() const
{
	int width = m_editor->lineNumberAreaWidth();
	return QSize(width, 0);
}

void LineNumberArea::paintEvent(QPaintEvent *paintEvent)
{
	m_editor->lineNumberAreaPaintEvent(paintEvent);
}

Editor::Editor(QWidget *parent) : QPlainTextEdit(parent)
{
	setLineWrapMode(NoWrap);

	m_lineNumberArea = new LineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	highlightCurrentLine();
	updateLineNumberAreaWidth(0);

	m_highlighter = new Highlighter(this);
	m_highlighter->setDocument(document());

	QSettings settings;
	settings.beginGroup("Font");
		QFont font;
		settings.beginGroup("Editor");
			font.setFamily(settings.value("Family", "Courier New").toString());
			font.setPointSize(settings.value("PointSize", 10).toInt());
			font.setBold(settings.value("Bold", false).toBool());
			font.setBold(settings.value("Italic", false).toBool());
			QPlainTextEdit::setFont(font);
		settings.endGroup();
	settings.endGroup();
}

void Editor::setFont(const QFont &font)
{
	QSettings settings;
	settings.beginGroup("Font");
		settings.beginGroup("Editor");
			settings.setValue("Family", font.family());
			settings.setValue("PointSize", font.pointSize());
			settings.setValue("Bold", font.bold());
			settings.setValue("Italic", font.italic());
		settings.endGroup();
	settings.endGroup();

	QPlainTextEdit::setFont(font);
}

void Editor::lineNumberAreaPaintEvent(QPaintEvent *paintEvent)
{
	QPainter painter(m_lineNumberArea);
	painter.fillRect(paintEvent->rect(), Qt::lightGray);

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && top <= paintEvent->rect().bottom()) {
		if (block.isVisible() && bottom >= paintEvent->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(m_lineNumberArea->width() * 0.25,
				top,  m_lineNumberArea->width() * 0.5,
				fontMetrics().height(), Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		++blockNumber;
	}
}

int Editor::lineNumberAreaWidth() const
{
	int numDigits = 1;
	int max = qMax(1, blockCount());

	while (max >= 10) {
		max /= 10;
		++numDigits;
	}

	int digitWidth = fontMetrics().width(QLatin1Char('9'));
	int spaceRequired = digitWidth * numDigits;

	// We will leave some space blank for the number's left and right
	// margins (0.5 of number width for both, see lineNumberAreaPaintEvent)
	int actualSpace = spaceRequired * 2;

	return actualSpace;
}

void Editor::resizeEvent(QResizeEvent *resizeEvent)
{
	QPlainTextEdit::resizeEvent(resizeEvent);

	QRect cr = contentsRect();
	m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
		lineNumberAreaWidth(), cr.height()));
}

void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy) {
		m_lineNumberArea->scroll(0, dy);
	} else {
		m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
	}

	if (rect.contains(viewport()->rect())) {
		updateLineNumberAreaWidth(0);
	}
}

void Editor::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void Editor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::lightGray).lighter(120);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}
