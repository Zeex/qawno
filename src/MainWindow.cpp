#include <QApplication>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QIcon>
#include <QList>
#include <QMenuBar>
#include <QMessageBox>
#include <QRegExp>
#include <QSettings>
#include <QUrl>

#include "AboutDialog.h"
#include "CompilerOptionsDialog.h"
#include "EditorWidget.h"
#include "FindDialog.h"
#include "GoToDialog.h"
#include "MainWindow.h"
#include "OutputWidget.h"
#include "ReplaceDialog.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    editorWidget_(new EditorWidget(this)),
    outputWidget_(new OutputWidget(this)),
    lastFind_(0),
    lastReplace_(0)
{
  setWindowIcon(QIcon(":icons/pawn.ico"));

  setCentralWidget(editorWidget_);
  connect(editorWidget_, SIGNAL(textChanged()), SLOT(updateWindowTitle()));

  setAcceptDrops(true);
  editorWidget_->setAcceptDrops(false);

  QMenuBar *menuBar = new QMenuBar(this);
  setMenuBar(menuBar);

  QMenu *fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(tr("New"), this, SLOT(newFile()), QKeySequence("Ctrl+N"));
  fileMenu->addAction(tr("Open"), this, SLOT(openFile()), QKeySequence("Ctrl+O"));
  fileMenu->addAction(tr("Close"), this, SLOT(closeFile()), QKeySequence("Ctrl+W"));
  fileMenu->addSeparator();
  fileMenu->addAction(tr("Save"), this, SLOT(saveFile()), QKeySequence("Ctrl+S"));
  fileMenu->addAction(tr("Save as..."), this, SLOT(saveFileAs()), QKeySequence("Ctrl+Shift+S"));
  fileMenu->addSeparator();
  fileMenu->addAction(tr("Exit"), this, SLOT(exit()), QKeySequence("Ctrl+Q"));
  menuBar->addMenu(fileMenu);

  QMenu *editMenu = new QMenu(tr("&Edit"), this);
  editMenu->addAction(tr("Undo"), editorWidget_, SLOT(undo()), QKeySequence("Ctrl+Z"));
  editMenu->addAction(tr("Redo"), editorWidget_, SLOT(redo()), QKeySequence("Ctrl+Y"));
  editMenu->addSeparator();
  editMenu->addAction(tr("Cut"), editorWidget_, SLOT(cut()), QKeySequence("Ctrl+X"));
  editMenu->addAction(tr("Copy"), editorWidget_, SLOT(copy()), QKeySequence("Ctrl+C"));
  editMenu->addAction(tr("Paste"), editorWidget_, SLOT(paste()), QKeySequence("Ctrl+V"));
  editMenu->addSeparator();
  editMenu->addAction(tr("Find..."), this, SLOT(find()), QKeySequence("Ctrl+F"));
  editMenu->addAction(tr("Find next"), this, SLOT(findNext()), QKeySequence("F3"));
  editMenu->addAction(tr("Replace..."), this, SLOT(replace()), QKeySequence("Ctrl+H"));
  editMenu->addSeparator();
  editMenu->addAction(tr("Go to line..."), this, SLOT(goToLine()), QKeySequence("Ctrl+G"));
  menuBar->addMenu(editMenu);

  QMenu *buildMenu = new QMenu(tr("&Build"), this);
  buildMenu->addAction(tr("Compile"), this, SLOT(compile()), QKeySequence("F5"));
  menuBar->addMenu(buildMenu);

  QMenu *optionsMenu = new QMenu(tr("&Settings"), this);
    QMenu *fontMenu = optionsMenu->addMenu(tr("Font"));
    fontMenu->addAction(tr("Editor"), this, SLOT(selectEditorFont()));
    fontMenu->addAction(tr("Output"), this, SLOT(selectOutputFont()));
  optionsMenu->addAction(tr("Compiler"), this, SLOT(setupCompiler()));
  menuBar->addMenu(optionsMenu);

  QMenu *helpMenu = new QMenu(tr("&Help"), this);
  QString appName = QCoreApplication::applicationName();
  helpMenu->addAction(tr("About %1...").arg(appName), this, SLOT(about()));
  helpMenu->addAction(tr("About Qt.."), this, SLOT(aboutQt()));
  menuBar->addMenu(helpMenu);

  outputWidget_->setReadOnly(true);

  QDockWidget *outputDock = new QDockWidget(tr("Output"), this);
  outputDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  outputDock->setWidget(outputWidget_);
  addDockWidget(Qt::BottomDockWidgetArea, outputDock);

  connect(&compiler_, SIGNAL(finished(int)), this, SLOT(compiled(int)));

  connect(this, SIGNAL(loaded()), SLOT(loadSettings()));
  connect(this, SIGNAL(closed()), SLOT(saveSettings()));
  connect(this, SIGNAL(loaded()), &compiler_, SLOT(loadSettings()));
  connect(this, SIGNAL(loaded()), &compiler_, SLOT(saveSettings()));
  connect(this, SIGNAL(loaded()), editorWidget_, SLOT(loadSettings()));
  connect(this, SIGNAL(closed()), editorWidget_, SLOT(saveSettings()));
  connect(this, SIGNAL(loaded()), outputWidget_, SLOT(loadSettings()));
  connect(this, SIGNAL(closed()), outputWidget_, SLOT(saveSettings()));

  // Open the file specified at the command line, if any.
  if (QApplication::instance()->arguments().size() > 1) {
    readFile(QApplication::instance()->arguments()[1]);
  }

  emit loaded();
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
  if (!compiler_.works()) {
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

  compiler_.run(fileName_);
}

void MainWindow::compiled(int /*exitCode*/) {
  outputWidget_->clear();

  QString command = compiler_.getCommandLine(fileName_);
  outputWidget_->appendPlainText(command);
  outputWidget_->appendPlainText("\n");

  QString output = compiler_.output();
  outputWidget_->appendPlainText(output);
}

void MainWindow::setupCompiler() {
  CompilerOptionsDialog dialog;

  dialog.setCompilerPath(compiler_.path());
  dialog.setCompilerOptions(compiler_.options().join(" "));

  dialog.exec();

  if (dialog.result() == QDialog::Accepted) {
    compiler_.setPath(dialog.getCompilerPath());
    compiler_.setOptions(dialog.getCompilerOptions());
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

void MainWindow::closeEvent(QCloseEvent *event) {
  if (isSafeToClose()) {
    emit closed();
    event->accept();
  } else {
    event->ignore();
  }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void MainWindow::dropEvent(QDropEvent *event) {
  QList<QUrl> urls = event->mimeData()->urls();
  foreach (QUrl url, urls) {
    if (url.isLocalFile()) {
      if (closeFile()) {
        readFile(url.toLocalFile());
      }
      break;
    }
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
  } else {
    file.write(editorWidget_->toPlainText().toLatin1());
    editorWidget_->document()->setModified(false);
    updateWindowTitle();
  }
}

void MainWindow::loadSettings() {
  QSettings settings;
  resize(settings.value("Window/Size", QSize(640, 480)).toSize());
  if (settings.value("Window/Maximized", false).toBool()) {
    setWindowState(Qt::WindowMaximized);
  }
}

void MainWindow::saveSettings() {
  QSettings settings;
  settings.setValue("Window/Maximized", isMaximized());
  if (!isMaximized()) {
    settings.setValue("Window/Size", size());
  }
}
