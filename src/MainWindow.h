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

  void setupCompiler();
  void selectEditorFont();
  void selectOutputFont();

  void about();
  void aboutQt();

  void refreshTitle();

 private:
  bool editingNewFile() const;
  bool fileIsModified() const;
  bool fileIsEmpty() const;
  bool fileIsCompletelyEmpty() const;

 private:
  Ui::MainWindow *ui_;

 private:
  QString fileName_;
};

#endif // MAINWINDOW_H
