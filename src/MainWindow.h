#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QMenuBar;
class QPlainTextEdit;
class QProcess;
class QSplitter;
class QTableWidget;
class QTextEdit;

class Compiler;
class EditorWidget;
class OutputWidget;
class FindDialog;
class ReplaceDialog;

class MainWindow: public QMainWindow {
 Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);

 protected:
  void closeEvent(QCloseEvent *closeEvent);

 private slots:
  bool newFile();
  bool openFile();
  bool closeFile();
  bool saveFile();
  bool saveFileAs();
  bool exit();
  void find();
  void findNext();
  void replace();
  void goToLine();
  void selectEditorFont();
  void selectOutputFont();
  void compile();
  void setupCompiler();
  void about();
  void aboutQt();
  void compiled(int);
  void updateWindowTitle();

 private:
  bool isSafeToClose();

  void readFile(QString fileName);
  void writeFile(QString fileName);

  void readSettings();
  void writeSettings();

 private:
  QString  fileName_; // Currently edited file

  Compiler *compiler_;

  EditorWidget *editorWidget_;
  OutputWidget *outputWidget_;

  FindDialog *lastFind_;
  ReplaceDialog *lastReplace_;
};

#endif // MAINWINDOW_H
