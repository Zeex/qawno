#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Compiler.h"
#include "FindDialog.h"
#include "ReplaceDialog.h"

class EditorWidget;
class OutputWidget;

namespace Ui {
  class MainWindow;
}

class MainWindow: public QMainWindow {
 Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 protected:
  void closeEvent(QCloseEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);

 private slots:
  bool newFile();

  bool openFile();
  bool closeFile();

  bool loadFile(QString fileName);
  bool saveFile();
  bool saveFileAs();

  void find();
  void findNext(bool wrapAround = true);
  void goToLine();

  void compile();
  void postCompile(int);

  void setupCompiler();
  void selectEditorFont();
  void selectOutputFont();

  void about();
  void aboutQt();

  void refreshTitle();

 private:
  Ui::MainWindow *ui_;

 private:
  QString fileName_;
  Compiler compiler_;
};

#endif // MAINWINDOW_H
