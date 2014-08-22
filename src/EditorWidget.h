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

  enum IndentChar {
    IndentTab,
    IndentSpace
  };

  enum IndentDirection {
    Indent,
    Unindent
  };

  explicit EditorWidget(QWidget *parent = 0);
  ~EditorWidget() override;

  IndentChar indentChar() const { return indentChar_; }
  void setIndentChar(IndentChar c) { indentChar_ = c; }

  int indentSize() const { return indentSize_; }
  void setIndentSize(int size) { indentSize_ = size; }

  int tabSize() const { return tabSize_; }
  void setTabSize(int size) {
    setTabStopWidth(fontMetrics().width(' ') * (tabSize_ = size));
  }

 public slots:
  void jumpToLine(long line);

 protected:
   void resizeEvent(QResizeEvent *event) override;
   void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void highlightCurrentLine();

 private:
  void editSelection(QTextCursor cursor,
                     std::function<void(QTextCursor cursor)> callback);

  void indentSelection(QTextCursor cursor);
  void unindentSelection(QTextCursor cursor);

 private:
  EditorLineNumberWidget lineNumberArea_;
  SyntaxHighlighter highlighter_;
  IndentChar indentChar_;
  int indentSize_;
  int tabSize_;
};

#endif // EDITORWIDGET_h
