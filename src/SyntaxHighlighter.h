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

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SyntaxHighlighter: public QSyntaxHighlighter {
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

  explicit SyntaxHighlighter(QObject *parent);
  ~SyntaxHighlighter() override;

  void highlightBlock(const QString &text) override;

  const ColorScheme &colorScheme() const;
  void setColorScheme(const ColorScheme &scheme);

 private:
  bool isIdentifierFirstChar(QChar c);
  bool isIdentifierChar(QChar c);
  bool isHexDigit(QChar c);
  bool isKeyword(const QString &s);

  QStringList keywords_;

  ColorScheme colorScheme_;
};

#endif // HIGHLIGHTER_H
