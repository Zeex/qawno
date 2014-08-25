// This file is part of qawno.
//
// qawno is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// qawno is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with qawno.  If not, see <http://www.gnu.org/licenses/>.

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

SyntaxHighlighter::SyntaxHighlighter(QObject *parent)
  : QSyntaxHighlighter(parent)
{
  colorScheme_ = defaultColorScheme;

  keywords_
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

SyntaxHighlighter::~SyntaxHighlighter() {
  // nothing
}

const SyntaxHighlighter::ColorScheme &SyntaxHighlighter::colorScheme() const {
  return colorScheme_;
}

void SyntaxHighlighter::setColorScheme(const ColorScheme &scheme) {
  colorScheme_ = scheme;
}

bool SyntaxHighlighter::isIdentifierFirstChar(QChar c) {
  return c.isLetter() || c == '_' || c == '@';
}

bool SyntaxHighlighter::isIdentifierChar(QChar c) {
  return isIdentifierFirstChar(c) || c.isDigit();
}

bool SyntaxHighlighter::isHexDigit(QChar c) {
  if (c.isDigit()) {
    return true;
  }
  char cc = c.toLatin1();
  return (cc >= 'a' && cc <= 'f') || (cc >= 'A' && cc <= 'F') || cc == 'x';
}

bool SyntaxHighlighter::isKeyword(const QString &s) {
  foreach (const QString &keyword, keywords_) {
    if (s == keyword) {
      return true;
    }
  }
  return false;
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
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
      setFormat(i, 1, colorScheme_.cComment);
      break;
    case CommentBegin:
      if (text[i] == '/') {
        setFormat(i - 1, text.length() - i + 1, colorScheme_.cppComment);
        state = Unknown;
        goto end;
      } else if (text[i] == '*') {
        setFormat(i - 1, 2, colorScheme_.cComment);
        state = Comment;
      } else {
        state = Unknown;
      }
      break;
    case CommentEnd:
      setFormat(i, 1, colorScheme_.cComment);
      if (text[i] == '/') {
        state = Unknown;
      } else {
        state = Comment;
      }
      break;
    case Identifier:
      if (isIdentifierChar(text[i])) {
        setFormat(i, 1, colorScheme_.identifier);
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
        setFormat(start + 1, ident.length(), colorScheme_.keyword);
      }
      state = Unknown;
      break;
    }
    case NumericLiteral:
      if (text[i].isDigit() || isHexDigit(text[i]) || text[i] == '.') {
        setFormat(i, 1, colorScheme_.number);
      } else {
        --i;
        state = Unknown;
      }
      break;
    case CharacterLiteral:
      setFormat(i, 1, colorScheme_.character);
      if (text[i] == '\'') {
        state = Unknown;
      }
      break;
    case StringLiteral:
      setFormat(i, 1, colorScheme_.string);
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
          setFormat(i, 1, colorScheme_.preprocessor);
        }
      }
      break;
    case PreprocessorNextLine:
      break;
    default:
      if (text[i] == '\'') {
        state = CharacterLiteral;
        setFormat(i, 1, colorScheme_.character);
      } else if (isIdentifierFirstChar(text[i])) {
        state = Identifier;
        setFormat(i, 1, colorScheme_.identifier);
      } else if (text[i].isDigit()) {
        setFormat(i, 1, colorScheme_.number);
        state = NumericLiteral;
      } else if (text[i] == '/') {
        state = CommentBegin;
      } else if (text[i] == '\"') {
        state = StringLiteral;
        setFormat(i, 1, colorScheme_.string);
      } else if (text[i] == '#') {
        state = Preprocessor;
        setFormat(i, 1, colorScheme_.preprocessor);
      } else if (text[i] == '<') {
        if (text.contains("#include")) {
          int count = 0;
          while (count < text.length()) {
            ++count;
          }
          setFormat(i, count, colorScheme_.string);
          i += count;
        }
      }
      break;
    }
  }

end:
  setCurrentBlockState((int)state);
}
