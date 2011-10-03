#include <QMessageBox>

#include "highlighter.h"

Highlighter::Highlighter(QObject *parent) :
	QSyntaxHighlighter(parent)
{
}

void Highlighter::highlightBlock(const QString &text)
{
	QTextCharFormat keywordFormat;
	keywordFormat.setFontWeight(QFont::Bold);
	keywordFormat.setForeground(Qt::darkMagenta);

	setFormat(0, text.length(), Qt::darkMagenta);
/*
	QString pattern = "\\bpublic\\b";
	QRegExp regex(pattern);

	int index = text.indexOf(regex);
	while (index >= 0) {
		int length = regex.matchedLength();
		setFormat(index, length, keywordFormat);
		index = text.indexOf(regex, index + length);
	}
*/
}
