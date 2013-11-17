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

  void highlightBlock(const QString &text);

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
