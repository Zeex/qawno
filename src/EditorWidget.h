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
