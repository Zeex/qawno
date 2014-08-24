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
  void on_actionIndentTab2_triggered();
  void on_actionIndentTab4_triggered();
  void on_actionIndentTab8_triggered();
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
