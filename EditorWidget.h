#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QObject>
#include <QPlainTextEdit>
#include <QWidget>

class EditorWidget;
class SyntaxHighlighter;

class LineNumberArea : public QWidget
{
	Q_OBJECT
public:
	LineNumberArea(EditorWidget *editor);

	QSize sizeHint() const;

protected:
	void paintEvent(QPaintEvent *paintEvent);

private:
	EditorWidget *m_editor;
};

class EditorWidget : public QPlainTextEdit
{
	Q_OBJECT
	friend class LineNumberArea;

public:
	explicit EditorWidget(QWidget *parent = 0);
	~EditorWidget();

	void setCurrentLine(long line);

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
	SyntaxHighlighter *m_highlighter;
};

#endif // CODEEDIT_H
