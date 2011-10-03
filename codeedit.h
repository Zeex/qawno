#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QPlainTextEdit>

class CodeEdit;

class LineNumberArea : public QWidget
{
	Q_OBJECT
public:
	LineNumberArea(CodeEdit *editor);

	QSize sizeHint() const;

protected:
	void paintEvent(QPaintEvent *paintEvent);

private:
	CodeEdit *m_codeEdit;
};

class CodeEdit : public QPlainTextEdit
{
	Q_OBJECT
	friend class LineNumberArea;

public:
	explicit CodeEdit(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *e);

private:
	int lineNumberAreaWidth() const;
	void lineNumberAreaPaintEvent(QPaintEvent *paintEvent);

private slots:
	void updateLineNumberArea(const QRect &, int);
	void updateLineNumberAreaWidth(int blockCount);
	void highlightCurrentLine();

private:
	LineNumberArea *m_lineNumberArea;
};

#endif // CODEEDIT_H
