#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QPlainTextEdit>

class Editor;
class Highlighter;

class LineNumberArea : public QWidget
{
	Q_OBJECT
public:
	LineNumberArea(Editor *editor);

	QSize sizeHint() const;

protected:
	void paintEvent(QPaintEvent *paintEvent);

private:
	Editor *m_editor;
};

class Editor : public QPlainTextEdit
{
	Q_OBJECT
	friend class LineNumberArea;

public:
	explicit Editor(QWidget *parent = 0);

	void setFont(const QFont &font);

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
	Highlighter *m_highlighter;
};

#endif // CODEEDIT_H
