#include <QApplication>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QSettings>

#include "AboutDialog.h"
#include "Compiler.h"
#include "CompilerOptionsDialog.h"
#include "EditorWidget.h"
#include "FindDialog.h"
#include "GoToDialog.h"
#include "MainWindow.h"
#include "MenuBar.h"
#include "OutputWidget.h"
#include "ReplaceDialog.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    lastFind_(0),
    lastReplace_(0)
{
  editorWidget_ = new EditorWidget(this);

  setCentralWidget(editorWidget_);
  connect(editorWidget_, SIGNAL(textChanged()), SLOT(updateWindowTitle()));

  MenuBar *menuBar = new MenuBar(this);
  setMenuBar(menuBar);
  connect(menuBar->actions().fileNew, SIGNAL(triggered()), this, SLOT(newFile()));
  connect(menuBar->actions().fileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(menuBar->actions().fileClose, SIGNAL(triggered()), this, SLOT(closeFile()));
  connect(menuBar->actions().fileSave, SIGNAL(triggered()), this, SLOT(saveFile()));
  connect(menuBar->actions().fileSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));
  connect(menuBar->actions().fileExit, SIGNAL(triggered()), this, SLOT(exit()));
  connect(menuBar->actions().editUndo, SIGNAL(triggered()), editorWidget_, SLOT(undo()));
  connect(menuBar->actions().editRedo, SIGNAL(triggered()), editorWidget_, SLOT(redo()));
  connect(menuBar->actions().editCut, SIGNAL(triggered()), editorWidget_, SLOT(cut()));
  connect(menuBar->actions().editCopy, SIGNAL(triggered()), editorWidget_, SLOT(copy()));
  connect(menuBar->actions().editPaste, SIGNAL(triggered()), editorWidget_, SLOT(paste()));
  connect(menuBar->actions().editFind, SIGNAL(triggered()), this, SLOT(find()));
  connect(menuBar->actions().editFindNext, SIGNAL(triggered()), this, SLOT(findNext()));
  //connect(menuBar->actions().editReplace, SIGNAL(triggered()), this, SLOT(replace()));
  connect(menuBar->actions().editGoToLine, SIGNAL(triggered()), this, SLOT(goToLine()));
  connect(menuBar->actions().buildCompile, SIGNAL(triggered()), this, SLOT(compile()));
  connect(menuBar->actions().optionsFontEditor, SIGNAL(triggered()), SLOT(selectEditorFont()));
  connect(menuBar->actions().optionsFontOutput, SIGNAL(triggered()), SLOT(selectOutputFont()));
  connect(menuBar->actions().optionsCompiler, SIGNAL(triggered()), SLOT(setupCompiler()));
  connect(menuBar->actions().helpAbout, SIGNAL(triggered()), SLOT(about()));
  connect(menuBar->actions().helpAboutQt, SIGNAL(triggered()), SLOT(aboutQt()));

  QDockWidget *outputDock = new QDockWidget(tr("Output"), this);
  outputDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  outputWidget_ = new OutputWidget(this);
  outputWidget_->setReadOnly(true);
  outputDock->setWidget(outputWidget_);
  addDockWidget(Qt::BottomDockWidgetArea, outputDock);

  compiler_ = new Compiler(this);
  connect(compiler_, SIGNAL(finished(int)), this, SLOT(compiled(int)));

  // Restore window state
  readSettings();

  // Open file specified at command line, if any
  if (QApplication::instance()->arguments().size() > 1) {
    readFile(QApplication::instance()->arguments()[1]);
  }
}

bool MainWindow::newFile() {
  return closeFile();
}

bool MainWindow::openFile() {
  if (closeFile()) {
    QFileDialog openDialog(this);
    QString fileName = openDialog.getOpenFileName(this, tr("Open file"), "",
                                              tr("Pawn scripts (*.pwn *.inc)"));
    readFile(fileName);
    return true;
  }
  return false;
}

bool MainWindow::isSafeToClose() {
  if (editorWidget_->document()->isModified() &&
      !editorWidget_->document()->isEmpty())
  {
    QString message;

    if (!fileName_.isEmpty()) {
      message = tr("Save changes to %1?").arg(fileName_);
    } else {
      message = tr("Save changes to a new file?");
    }

    int button = QMessageBox::question(this,
                                       QCoreApplication::applicationName(),
                                       message, QMessageBox::Yes |
                                                QMessageBox::No  |
                                                QMessageBox::Cancel);

    switch (button) {
      case QMessageBox::Yes:
        saveFile();
        if (editorWidget_->document()->isModified()) {
          // Save file failed or cancelled
          return false;
        }
        return true;
      case QMessageBox::No:
        break;
      case QMessageBox::Cancel:
        return false;
    }
  }

  return true;
}

bool MainWindow::closeFile() {
  if (isSafeToClose()) {
    editorWidget_->clear();
    fileName_.clear();
    return true;
  }
  return false;
}

bool MainWindow::saveFile() {
  if (editorWidget_->document()->isEmpty()) {
    return false;
  }

  if (fileName_.isEmpty()) {
    saveFileAs();
    return false;
  }

  writeFile(fileName_);
  return true;
}

bool MainWindow::saveFileAs() {
  if (editorWidget_->document()->isEmpty()) {
    return false;
  }

  QFileDialog saveDialog;
  QString fileName = saveDialog.getSaveFileName(this,
  tr("Save file as"), "", tr("Pawn scripts (*.pwn *.inc)"));

  if (!fileName.isEmpty()) {
    fileName_ = fileName;
    return saveFile();
  }

  return false;
}

bool MainWindow::exit() {
  if (closeFile()) {
    QApplication::exit();
    return true;
  }
  return false;
}

void MainWindow::find() {
  if (lastFind_ != 0) {
  delete lastFind_;
  }

  lastFind_ = new FindDialog;
  lastFind_->exec();

  emit(findNext());
}

void MainWindow::findNext() {
  if (lastFind_ == 0) {
    return;
  }

  QTextDocument::FindFlags flags;
  if (lastFind_->matchCase()) {
    flags |= QTextDocument::FindCaseSensitively;
  }
  if (lastFind_->matchWholeWords()) {
    flags |= QTextDocument::FindWholeWords;
  }
  if (lastFind_->searchBackwards()) {
    flags |= QTextDocument::FindBackward;
  }

  QTextCursor cursor;

  if (lastFind_->useRegexp()) {
    QRegExp regexp(lastFind_->findWhatText(),
      lastFind_->matchCase() ? Qt::CaseSensitive : Qt::CaseInsensitive);
    cursor = editorWidget_->document()->find(regexp, editorWidget_->textCursor(), flags);
  } else {
    cursor = editorWidget_->document()->find(lastFind_->findWhatText(),
                                        editorWidget_->textCursor(), flags);
  }

  if (!cursor.isNull()) {
    editorWidget_->setTextCursor(cursor);
  }
}

void MainWindow::replace() {
  if (lastReplace_ != 0) {
    delete lastReplace_;
  }

  lastReplace_ = new ReplaceDialog;
  lastReplace_->exec();

  //emit(replaceNext());
}

void MainWindow::goToLine() {
  GoToDialog dialog;
  dialog.exec();

  long line = dialog.targetLineNumber();
  if (line >= 0 && line < editorWidget_->blockCount()) {
    editorWidget_->setCurrentLine(line);
  }
}

void MainWindow::selectEditorFont() {
  QFontDialog fontDialog(this);

  bool ok = false;
  QFont newFont = fontDialog.getFont(&ok, editorWidget_->font(), this,
                                     tr("Select editor font"));

  if (ok) {
    editorWidget_->setFont(newFont);
  }
}

void MainWindow::selectOutputFont() {
  QFontDialog fontDialog(this);

  bool ok = false;
  QFont newFont = fontDialog.getFont(&ok, outputWidget_->font(), this,
  tr("Select output font"));

  if (ok) {
  outputWidget_->setFont(newFont);
  }
}

void MainWindow::compile() {
  if (!compiler_->test()) {
    QString message = tr("Pawn compiler is not set or missing.\n"
                         "Do you want to set compiler path now?");
    int button = QMessageBox::warning(this, QCoreApplication::applicationName(),
                                      message , QMessageBox::Yes |
                                                QMessageBox::No);
    if (button != QMessageBox::No) {
      setupCompiler();
    }
    return;
  }

  if (editorWidget_->toPlainText().isEmpty()) {
    outputWidget_->appendPlainText(tr("Nothing to compile!"));
    return;
  }

  if (fileName_.isEmpty()) {
    saveFileAs();
    return;
  }

  compiler_->run(fileName_);
}

void MainWindow::compiled(int /*exitCode*/) {
  outputWidget_->clear();

  QString command = compiler_->commandLineFor(fileName_);
  outputWidget_->appendPlainText(command);
  outputWidget_->appendPlainText("\n");

  QString output = compiler_->output();
  outputWidget_->appendPlainText(output);
}

void MainWindow::setupCompiler() {
  CompilerOptionsDialog dialog;

  dialog.setCompilerPath(compiler_->path());
  dialog.setCompilerOptions(compiler_->options().join(" "));

  dialog.exec();

  if (dialog.result() == QDialog::Accepted) {
    compiler_->setPath(dialog.getCompilerPath());
    compiler_->setOptions(dialog.getCompilerOptions());
  }
}

void MainWindow::about() {
  AboutDialog dialog;
  dialog.exec();
}

void MainWindow::aboutQt() {
  QMessageBox::aboutQt(this);
}

void MainWindow::updateWindowTitle() {
  QString title;
  if (!fileName_.isEmpty()) {
    title.append(QFileInfo(fileName_).fileName());
    if (editorWidget_->document()->isModified()) {
      title.append("*");
    }
    title.append(" - ");
  }
  title.append(QCoreApplication::applicationName());
  setWindowTitle(title);
}

void MainWindow::closeEvent(QCloseEvent *closeEvent) {
  if (isSafeToClose()) {
    writeSettings();
    closeEvent->accept();
  } else {
    closeEvent->ignore();
  }
}

void MainWindow::readFile(QString fileName) {
  if (!fileName.isEmpty()) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
      QString message = tr("Could not open %1: %2.").arg(fileName)
                                                    .arg(file.errorString());
      QMessageBox::critical(this, QCoreApplication::applicationName(),
                            message, QMessageBox::Ok);
    } else {
      fileName_ = fileName;
      editorWidget_->setPlainText(file.readAll());
      editorWidget_->document()->setModified(false);
      updateWindowTitle();
    }
  }
}

void MainWindow::writeFile(QString fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, QCoreApplication::applicationName(),
      tr("Could not save to %1: %2.")
      .arg(fileName)
      .arg(file.errorString()),
      QMessageBox::Ok);
    return;
  }

  file.write(editorWidget_->toPlainText().toLatin1());
  editorWidget_->document()->setModified(false);
  updateWindowTitle();
}

void MainWindow::readSettings() {
  QSettings settings;

  settings.beginGroup("Widgets");
    settings.beginGroup("MainWindow");
      resize(settings.value("Size", QSize(640, 480)).toSize());
      move(settings.value("Pos").toPoint());
      if (settings.value("Maximized", false).toBool()) {
        setWindowState(Qt::WindowMaximized);
      }
    settings.endGroup();
  settings.endGroup();

  settings.beginGroup("Editor");
    editorWidget_->setTabStop(settings.value("TabStop").toInt());
  settings.endGroup();
}

void MainWindow::writeSettings() {
  QSettings settings;

  settings.beginGroup("Widgets");
    settings.beginGroup("MainWindow");
      settings.setValue("Maximized", isMaximized());
      if (!isMaximized()) {
        settings.setValue("Size", size());
        settings.setValue("Pos", pos());
      }
    settings.endGroup();
  settings.endGroup();

  settings.beginGroup("Editor");
    settings.setValue("TabStop", editorWidget_->tabStop());
  settings.endGroup();
}
