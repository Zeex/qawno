#include <QtGui>

#include "codeedit.h"

LineNumberArea::LineNumberArea(CodeEdit *edit)
	: QWidget(edit)
	, m_codeEdit(edit)
{
}

QSize LineNumberArea::sizeHint() const
{
	int width = m_codeEdit->lineNumberAreaWidth();
	return QSize(width, 0);
}

void LineNumberArea::paintEvent(QPaintEvent *paintEvent)
{
	m_codeEdit->lineNumberAreaPaintEvent(paintEvent);
}

CodeEdit::CodeEdit(QWidget *parent) : QPlainTextEdit(parent)
{
	setLineWrapMode(NoWrap);

	m_lineNumberArea = new LineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	highlightCurrentLine();
	updateLineNumberAreaWidth(0);
}

void CodeEdit::lineNumberAreaPaintEvent(QPaintEvent *paintEvent)
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

int CodeEdit::lineNumberAreaWidth() const
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

void CodeEdit::resizeEvent(QResizeEvent *resizeEvent)
{
	QPlainTextEdit::resizeEvent(resizeEvent);

	QRect cr = contentsRect();
	m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
		lineNumberAreaWidth(), cr.height()));
}

void CodeEdit::updateLineNumberArea(const QRect &rect, int dy)
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

void CodeEdit::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEdit::highlightCurrentLine()
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
