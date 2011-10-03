#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QRegExp>
#include <QTextCharFormat>
#include <QStack>
#include <QSyntaxHighlighter>
#include <QVector>

class PawnHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	struct ColorScheme {
		QColor defaultColor;
		QColor keyword;
		QColor cComment;
		QColor cppComment;
		QColor identifier;
		QColor number;
		QColor character;
		QColor string;
		QColor preprocessor;
	};

	explicit PawnHighlighter(QObject *parent = 0);

	void highlightBlock(const QString &text);

	const ColorScheme &colorScheme() const;
	void setColorScheme(const ColorScheme &scheme);

private:
	struct HighlightingRule {
		QRegExp         regexp;
		QTextCharFormat format;
	};

	ColorScheme m_colorScheme;
};

#endif // HIGHLIGHTER_H
