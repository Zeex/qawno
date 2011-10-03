#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	explicit Highlighter(QObject *parent = 0);

	void highlightBlock(const QString &text);
};

#endif // HIGHLIGHTER_H
