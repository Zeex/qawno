#ifndef EDITORWIDGET_h
#define EDITORWIDGET_h

#include <QPlainTextEdit>

#include "SyntaxHighlighter.h"

class EditorWidget;

class EditorLineNumberArea: public QWidget {
 Q_OBJECT

 public:
  EditorLineNumberArea(EditorWidget *editor);
  ~EditorLineNumberArea()  override;

  EditorWidget *editor() const;
  QSize sizeHint() const override;

 public slots:
  void update(const QRect &rect, int dy);
  void updateWidth(int blockCount = 0);
  void updateGeometry();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
};

class EditorWidget: public QPlainTextEdit {
 Q_OBJECT

 public:
  friend class EditorLineNumberArea;

  explicit EditorWidget(QWidget *parent = 0);
  ~EditorWidget()  override;

  int tabStop() const;
  void setTabStop(int chars);

 public slots:
  void jumpToLine(long line);

 protected:
   void resizeEvent(QResizeEvent *event)  override;

 private slots:
  void highlightCurrentLine();

 private:
  int tabStop_;

 private:
  EditorLineNumberArea lineNumberArea_;
  SyntaxHighlighter highlighter_;
};

#endif // EDITORWIDGET_h
