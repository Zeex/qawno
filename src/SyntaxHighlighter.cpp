// TODO: more robust parser

#include "SyntaxHighlighter.h"

static SyntaxHighlighter::ColorScheme defaultColorScheme = {
	Qt::darkBlue,
	Qt::blue,
	Qt::darkGreen,
	Qt::darkGreen,
	Qt::black,
	Qt::darkMagenta,
	Qt::darkMagenta,
	Qt::darkRed,
	Qt::blue
};

SyntaxHighlighter::SyntaxHighlighter(QObject *parent) :
	QSyntaxHighlighter(parent)
{
	m_colorScheme = defaultColorScheme;

	m_keywords
		<< "const"
		<< "defined"
		<< "do"
		<< "else"
		<< "enum"
		<< "false"
		<< "for"
		<< "forward"
		<< "if"
		<< "native"
		<< "new"
		<< "public"
		<< "return"
		<< "sizeof"
		<< "static"
		<< "stock"
		<< "tagof"
		<< "true"
		<< "while";
}

const SyntaxHighlighter::ColorScheme &SyntaxHighlighter::colorScheme() const {
	return m_colorScheme;
}

void SyntaxHighlighter::setColorScheme(const ColorScheme &scheme) {
	m_colorScheme = scheme;
}

bool SyntaxHighlighter::isIdentifierFirstChar(QChar c)
{
	return c.isLetter() || c == '_' || c == '@';
}

bool SyntaxHighlighter::isIdentifierChar(QChar c)
{
	return isIdentifierFirstChar(c) || c.isDigit();
}

bool SyntaxHighlighter::isHexDigit(QChar c)
{
	if (c.isDigit()) {
		return true;
	}
	char cc = c.toLatin1();
	return (cc >= 'a' && cc <= 'f') || (cc >= 'A' && cc <= 'F') || cc == 'x';
}

bool SyntaxHighlighter::isKeyword(const QString &s)
{
	foreach (const QString &keyword, m_keywords) {
		if (s == keyword) {
			return true;
		}
	}
	return false;
}

// Simple hand-written highlighter (with no regexps!)
void SyntaxHighlighter::highlightBlock(const QString &text)
{
	setFormat(0, text.length(), defaultColorScheme.defaultColor);

	enum State {
		Unknown = -1,
		CommentBegin,
		Comment,
		CommentEnd,
		Identifier,
		IdentifierEnd,
		NumericLiteral,
		CharacterLiteral,
		StringLiteral,
		Preprocessor,
		PreprocessorNextLine
	};

	State state = (State)previousBlockState();

	for (int i = 0; i < text.length(); ++i) {
		switch (state) {
		case Comment:
			if (text[i] == '*') {
				state = CommentEnd;
			}
			setFormat(i, 1, m_colorScheme.cComment);
			break;
		case CommentBegin:
			if (text[i] == '/') {
				setFormat(i - 1, text.length() - i + 1, m_colorScheme.cppComment);
				state = Unknown;
				goto end;
			} else if (text[i] == '*') {
				setFormat(i - 1, 2, m_colorScheme.cComment);
				state = Comment;
			} else {
				state = Unknown;
			}
			break;
		case CommentEnd:
			setFormat(i, 1, m_colorScheme.cComment);
			if (text[i] == '/') {
				state = Unknown;
			} else {
				state = Comment;
			}
			break;
		case Identifier:
			if (isIdentifierChar(text[i])) {
				setFormat(i, 1, m_colorScheme.identifier);
			} else {
				--i;
				state = IdentifierEnd;
			}
			break;
		case IdentifierEnd: {
			QString ident;
			int start = i - 1;
			while (start >= 0 && isIdentifierChar(text[start])) {
				ident.prepend(text[start--]);
			}
			if (isKeyword(ident)) {
				setFormat(start + 1, ident.length(), m_colorScheme.keyword);
			}
			state = Unknown;
			break;
		}
		case NumericLiteral:
			if (text[i].isDigit() || isHexDigit(text[i]) || text[i] == '.') {
				setFormat(i, 1, m_colorScheme.number);
			} else {
				--i;
				state = Unknown;
			}
			break;
		case CharacterLiteral:
			setFormat(i, 1, m_colorScheme.character);
			if (text[i] == '\'') {
				state = Unknown;
			}
			break;
		case StringLiteral:
			setFormat(i, 1, m_colorScheme.string);
			if (text[i] == '\"') {
				state = Unknown;
			}
			break;
		case Preprocessor:
			if (text[i] == '\\') {
				state = PreprocessorNextLine;
				goto end;
			} else {
				if (text[i].isSpace()) {
					state = Unknown;
				} else {
					setFormat(i, 1, m_colorScheme.preprocessor);
				}
			}
			break;
		case PreprocessorNextLine:
			break;
		default:
			if (text[i] == '\'') {
				state = CharacterLiteral;
				setFormat(i, 1, m_colorScheme.character);
			} else if (isIdentifierFirstChar(text[i])) {
				state = Identifier;
				setFormat(i, 1, m_colorScheme.identifier);
			} else if (text[i].isDigit()) {
				setFormat(i, 1, m_colorScheme.number);
				state = NumericLiteral;
			} else if (text[i] == '/') {
				state = CommentBegin;
			} else if (text[i] == '\"') {
				state = StringLiteral;
				setFormat(i, 1, m_colorScheme.string);
			} else if (text[i] == '#') {
				state = Preprocessor;
				setFormat(i, 1, m_colorScheme.preprocessor);
			} else if (text[i] == '<') {
				if (text.contains("#include")) {
					int count = 0;
					while (count < text.length()) {
						++count;
					}
					setFormat(i, count, m_colorScheme.string);
					i += count;
				}
			}
			break;
		}
	}

end:
	setCurrentBlockState((int)state);
}
