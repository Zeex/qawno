#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QPlainTextEdit>

#include "SyntaxHighlighter.h"

class EditorWidget;

class EditorLineNumberArea: public QWidget {
 Q_OBJECT

 public:
  EditorLineNumberArea(EditorWidget *editor);
  virtual ~EditorLineNumberArea();

  EditorWidget *editor() const;
  virtual QSize sizeHint() const;

 public slots:
  void update(const QRect &rect, int dy);
  void updateWidth(int blockCount = 0);
  void updateGeometry();

 protected:
  virtual void paintEvent(QPaintEvent *event);
  virtual void resizeEvent(QResizeEvent *event);
};

class EditorWidget: public QPlainTextEdit {
 Q_OBJECT

 public:
  friend class EditorLineNumberArea;

  explicit EditorWidget(QWidget *parent = 0);
  virtual ~EditorWidget();

  int tabStop() const;
  void setTabStop(int chars);

 public slots:
  void jumpToLine(long line);

 protected:
  virtual void resizeEvent(QResizeEvent *event);

 private slots:
  void highlightCurrentLine();

 private:
  int tabStop_;

 private:
  EditorLineNumberArea lineNumberArea_;
  SyntaxHighlighter highlighter_;
};

#endif // CODEEDIT_H
