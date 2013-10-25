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

#include "ui_MainWindow.h"

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
    ui_(new Ui::MainWindow),
    lastFind_(0),
    lastReplace_(0)
{
  ui_->setupUi(this);

  connect(ui_->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
  connect(ui_->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(ui_->actionClose, SIGNAL(triggered()), this, SLOT(openClose()));
  connect(ui_->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
  connect(ui_->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));
  connect(ui_->actionFind, SIGNAL(triggered()), this, SLOT(find()));
  connect(ui_->actionFindNext, SIGNAL(triggered()), this, SLOT(findNext()));
  connect(ui_->actionCompile, SIGNAL(triggered()), this, SLOT(compile()));
  connect(ui_->actionEditorFont, SIGNAL(triggered()), this, SLOT(selectEditorFont()));
  connect(ui_->actionOutputFont, SIGNAL(triggered()), this, SLOT(selectOutputFont()));
  connect(ui_->actionCompilerSettings, SIGNAL(triggered()), this, SLOT(setupCompiler()));
  connect(ui_->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(ui_->actionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
  connect(ui_->actionGoToLine, SIGNAL(triggered()), this, SLOT(goToLine()));
  connect(ui_->editor, SIGNAL(textChanged()), SLOT(updateWindowTitle()));

  connect(&compiler_, SIGNAL(finished(int)), this, SLOT(compiled(int)));

  connect(this, SIGNAL(loaded()), SLOT(loadSettings()));
  connect(this, SIGNAL(closed()), SLOT(saveSettings()));
  connect(this, SIGNAL(loaded()), &compiler_, SLOT(loadSettings()));
  connect(this, SIGNAL(loaded()), &compiler_, SLOT(saveSettings()));
  connect(this, SIGNAL(loaded()), ui_->editor, SLOT(loadSettings()));
  connect(this, SIGNAL(closed()), ui_->editor, SLOT(saveSettings()));
  connect(this, SIGNAL(loaded()), ui_->output, SLOT(loadSettings()));
  connect(this, SIGNAL(closed()), ui_->output, SLOT(saveSettings()));

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
  if (ui_->editor->document()->isModified() &&
      !ui_->editor->document()->isEmpty())
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
        if (ui_->editor->document()->isModified()) {
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
    ui_->editor->clear();
    fileName_.clear();
    return true;
  }
  return false;
}

bool MainWindow::saveFile() {
  if (ui_->editor->document()->isEmpty()) {
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
  if (ui_->editor->document()->isEmpty()) {
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
    cursor = ui_->editor->document()->find(regexp, ui_->editor->textCursor(), flags);
  } else {
    cursor = ui_->editor->document()->find(lastFind_->findWhatText(),
                                        ui_->editor->textCursor(), flags);
  }

  if (!cursor.isNull()) {
    ui_->editor->setTextCursor(cursor);
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
  ui_->editor->jumpToLine(dialog.targetLineNumber());
}

void MainWindow::selectEditorFont() {
  QFontDialog fontDialog(this);

  bool ok = false;
  QFont newFont = fontDialog.getFont(&ok, ui_->editor->font(), this,
                                     tr("Select editor font"));

  if (ok) {
    ui_->editor->setFont(newFont);
  }
}

void MainWindow::selectOutputFont() {
  QFontDialog fontDialog(this);

  bool ok = false;
  QFont newFont = fontDialog.getFont(&ok, ui_->output->font(), this,
  tr("Select output font"));

  if (ok) {
  ui_->output->setFont(newFont);
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

  if (ui_->editor->toPlainText().isEmpty()) {
    ui_->output->appendPlainText(tr("Nothing to compile!"));
    return;
  }

  if (fileName_.isEmpty()) {
    saveFileAs();
    return;
  }

  compiler_.run(fileName_);
}

void MainWindow::compiled(int /*exitCode*/) {
  ui_->output->clear();

  QString command = compiler_.getCommandLine(fileName_);
  ui_->output->appendPlainText(command);
  ui_->output->appendPlainText("\n");

  QString output = compiler_.output();
  ui_->output->appendPlainText(output);
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
    if (ui_->editor->document()->isModified()) {
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
      ui_->editor->setPlainText(file.readAll());
      ui_->editor->document()->setModified(false);
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
    file.write(ui_->editor->toPlainText().toLatin1());
    ui_->editor->document()->setModified(false);
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
