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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow: public QMainWindow {
 Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow() override;

 protected:
  void closeEvent(QCloseEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

 private slots:
  void on_actionNew_triggered();
  void on_actionOpen_triggered();
  void on_actionClose_triggered();
  void on_actionSave_triggered();
  void on_actionSaveAs_triggered();

  void on_actionFind_triggered();
  void on_actionFindNext_triggered();
  void on_actionGoToLine_triggered();

  void on_actionCompile_triggered();

  void on_actionUseTabs_triggered();
  void on_actionUseSpaces_triggered();
  void on_actionTabWidth2_triggered();
  void on_actionTabWidth4_triggered();
  void on_actionTabWidth8_triggered();
  void on_actionIndentWidth2_triggered();
  void on_actionIndentWidth4_triggered();
  void on_actionIndentWidth8_triggered();

  void on_actionEditorFont_triggered();
  void on_actionOutputFont_triggered();

  void on_actionCompiler_triggered();

  void on_actionAbout_triggered();
  void on_actionAboutQt_triggered();

  void on_editor_textChanged();
  void on_editor_cursorPositionChanged();

 private:
  bool loadFile(const QString& fileName);
  bool isNewFile() const;
  bool isFileModified() const;
  bool isFileEmpty() const;

 private:
  Ui::MainWindow *ui_;

 private:
  QString fileName_;
  int findStart_ = 0;
  int findRound_ = 0;
};

#endif // MAINWINDOW_H
