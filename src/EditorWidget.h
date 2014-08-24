#ifndef EDITORWIDGET_h
#define EDITORWIDGET_h

#include <functional>

#include <QPlainTextEdit>

#include "SyntaxHighlighter.h"

class EditorWidget;

class EditorLineNumberWidget: public QWidget {
 Q_OBJECT

 public:
  EditorLineNumberWidget(EditorWidget *editor);
  ~EditorLineNumberWidget()  override;

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
  friend class EditorLineNumberWidget;

  enum IndentPolicy {
    IndentWithTabs,
    IndentWithSpaces
  };

  explicit EditorWidget(QWidget *parent = 0);
  ~EditorWidget() override;

  int tabWidth() const;
  void setTabWidth(int width);

  int indentWidth() const;
  void setIndentWidth(int width);

  IndentPolicy indentPolicy() const;
  void setIndentPolicy(IndentPolicy policy);

 public slots:
  void jumpToLine(long line);

 protected:
  void resizeEvent(QResizeEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void highlightCurrentLine();

 private:
  void editSelectedText(QTextCursor cursor,
                        std::function<void(QTextCursor cursor)> callback);

  void indentBlock(QTextCursor cursor);
  void indentSelectedText(QTextCursor cursor);
  void unindentSelectedText(QTextCursor cursor);

 private:
  EditorLineNumberWidget lineNumberArea_;
  SyntaxHighlighter highlighter_;
  int tabWidth_;
  int indentWidth_;
  IndentPolicy indentPolicy_;
};

#endif // EDITORWIDGET_h
