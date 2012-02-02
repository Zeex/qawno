#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QRegExp>
#include <QTextCharFormat>
#include <QStack>
#include <QSyntaxHighlighter>
#include <QVector>

class SyntaxHighlighter : public QSyntaxHighlighter
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

	explicit SyntaxHighlighter(QObject *parent = 0);

	void highlightBlock(const QString &text);

	const ColorScheme &colorScheme() const;
	void setColorScheme(const ColorScheme &scheme);

private:
	struct HighlightingRule {
		QRegExp         regexp;
		QTextCharFormat format;
	};

	bool isIdentifierFirstChar(QChar c);
	bool isIdentifierChar(QChar c);
	bool isHexDigit(QChar c);
	bool isKeyword(const QString &s);

	QStringList m_keywords;

	ColorScheme m_colorScheme;
};

#endif // HIGHLIGHTER_H
