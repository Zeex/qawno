#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QRegExp>
#include <QSettings>

#include "AboutDialog.h"
#include "Compiler.h"
#include "CompilerOptionsDialog.h"
#include "EditorWidget.h"
#include "FindDialog.h"
#include "GoToDialog.h"
#include "MainWindow.h"
#include "OutputWidget.h"
#include "ReplaceDialog.h"
#include "StatusBar.h"

#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
  ui_->setupUi(this);

  setStatusBar(new StatusBar(this));

  connect(ui_->actionNew, SIGNAL(triggered()), SLOT(newFile()));
  connect(ui_->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
  connect(ui_->actionClose, SIGNAL(triggered()), SLOT(closeFile()));
  connect(ui_->actionSave, SIGNAL(triggered()), SLOT(saveFile()));
  connect(ui_->actionSaveAs, SIGNAL(triggered()), SLOT(saveFileAs()));
  connect(ui_->actionFind, SIGNAL(triggered()), SLOT(find()));
  connect(ui_->actionFindNext, SIGNAL(triggered()), SLOT(findNext()));
  connect(ui_->actionCompile, SIGNAL(triggered()), SLOT(compile()));
  connect(ui_->actionEditorFont, SIGNAL(triggered()), SLOT(selectEditorFont()));
  connect(ui_->actionOutputFont, SIGNAL(triggered()), SLOT(selectOutputFont()));
  connect(ui_->actionCompilerSettings, SIGNAL(triggered()), SLOT(setupCompiler()));
  connect(ui_->actionAbout, SIGNAL(triggered()), SLOT(about()));
  connect(ui_->actionAboutQt, SIGNAL(triggered()), SLOT(aboutQt()));
  connect(ui_->actionGoToLine, SIGNAL(triggered()), SLOT(goToLine()));
  connect(ui_->editor, SIGNAL(textChanged()), SLOT(updateTitle()));
  connect(ui_->editor, SIGNAL(cursorPositionChanged()), SLOT(updateCursorStatus()));

  QSettings settings;

  resize(settings.value("Widgets/MainWindow/Size", QSize(640, 480)).toSize());
  if (settings.value("Widgets/MainWindow/Maximized", false).toBool()) {
    setWindowState(Qt::WindowMaximized);
  }

  if (QApplication::instance()->arguments().size() > 1) {
    loadFile(QApplication::instance()->arguments()[1]);
  }
}

MainWindow::~MainWindow() {
  QSettings settings;

  settings.setValue("Widgets/MainWindow/Maximized", isMaximized());
  if (!isMaximized()) {
    settings.setValue("Widgets/MainWindow/Size", size());
  }

  delete ui_;
}

bool MainWindow::newFile() {
  return closeFile();
}

bool MainWindow::openFile() {
  if (closeFile()) {
    QString caption = tr("Open file");
    QString filter = tr("Pawn scripts (*.pwn *.inc)");
    QString fileName = QFileDialog::getOpenFileName(this, caption, "", filter);
    loadFile(fileName);
    return true;
  }
  return false;
}

bool MainWindow::closeFile() {
  bool canClose = true;

  if (fileIsModified() && !fileIsEmpty()) {
    QString message = (!editingNewFile())
      ? tr("Save changes to %1?").arg(fileName_)
      : tr("Save changes to a new file?");
    int result = QMessageBox::question(this, QCoreApplication::applicationName(),
                                       message, QMessageBox::Yes |
                                                QMessageBox::No  |
                                                QMessageBox::Cancel);
    switch (result) {
      case QMessageBox::Yes:
        canClose = saveFile();
        break;
      case QMessageBox::No:
        canClose = true;
        break;
      case QMessageBox::Cancel:
        canClose = false;
        break;
    }
  }

  if (canClose) {
    ui_->editor->clear();
    fileName_.clear();
  }

  return canClose;
}

bool MainWindow::saveFile() {
  if (ui_->editor->document()->isEmpty()) {
    return false;
  } else {
    if (editingNewFile()) {
      return saveFileAs();
    } else {
      QFile file(fileName_);
      if (!file.open(QIODevice::WriteOnly)) {
        QString message =
          tr("Could not save to %1: %2.").arg(fileName_, file.errorString());
        QMessageBox::critical(this,
                              QCoreApplication::applicationName(),
                              message,
                              QMessageBox::Ok);
      } else {
        file.write(ui_->editor->toPlainText().toLatin1());
        ui_->editor->document()->setModified(false);
        updateTitle();
      }
    }
  }
  return true;
}

bool MainWindow::saveFileAs() {
  if (!ui_->editor->document()->isEmpty()) {
    QString caption = tr("Save file as");
    QString filter = tr("Pawn scripts (*.pwn *.inc)");
    QString fileName = QFileDialog::getSaveFileName(this, caption, "", filter);
    if (!fileName.isEmpty()) {
      fileName_ = fileName;
      return saveFile();
    }
  }
  return false;
}

void MainWindow::find() {
  bool find = false;
  {
    FindDialog dialog;
    dialog.exec();
    find = dialog.result() == QDialog::Accepted;
  }
  if (find) {
    emit(findNext(true));
  }
}

void MainWindow::findNext(bool wrapAround) {
  FindDialog dialog;
  QTextDocument::FindFlags flags;

  if (dialog.matchCase()) {
    flags |= QTextDocument::FindCaseSensitively;
  }
  if (dialog.matchWholeWords()) {
    flags |= QTextDocument::FindWholeWords;
  }
  if (dialog.searchBackwards()) {
    flags |= QTextDocument::FindBackward;
  }

  QTextCursor current = ui_->editor->textCursor();
  QTextCursor next;

  if (dialog.useRegExp()) {
    Qt::CaseSensitivity sens = dialog.matchCase()? Qt::CaseSensitive:
                                                   Qt::CaseInsensitive;
    QRegExp regexp(dialog.findWhatText(), sens);
    next = ui_->editor->document()->find(regexp, current, flags);
  } else {
    next = ui_->editor->document()->find(dialog.findWhatText(), current, flags);
  }

  bool found = !next.isNull();

  if (!found && wrapAround) {
    next = current;
    if (dialog.searchBackwards()) {
      next.movePosition(QTextCursor::End);
    } else {
      next.movePosition(QTextCursor::Start);
    }
  }

  if (found || (!found && wrapAround)) {
    ui_->editor->setTextCursor(next);
  }

  if (!found) {
    if (wrapAround) {
      findNext(false);
    } else {
      QString string = dialog.findWhatText();
      QString message = tr("No matches were found for \"%1\".").arg(string);
      QMessageBox::warning(this, QCoreApplication::applicationName(),
                           message, QMessageBox::Ok);
    }
  }
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

void MainWindow::setupCompiler() {
  Compiler compiler;
  CompilerOptionsDialog dialog;

  dialog.setCompilerPath(compiler.path());
  dialog.setCompilerOptions(compiler.options().join(" "));

  dialog.exec();

  if (dialog.result() == QDialog::Accepted) {
    compiler.setPath(dialog.compilerPath());
    compiler.setOptions(dialog.compilerOptions());
  }
}

void MainWindow::compile() {
  if (editingNewFile()) {
    saveFileAs();
  } else {
    Compiler compiler;
    ui_->output->clear();
    ui_->output->appendPlainText(compiler.commandFor(fileName_));
    ui_->output->appendPlainText("\n");
    compiler.run(fileName_);
    ui_->output->appendPlainText(compiler.output());
  }
}

void MainWindow::about() {
  AboutDialog dialog;
  dialog.exec();
}

void MainWindow::aboutQt() {
  QMessageBox::aboutQt(this);
}

void MainWindow::updateTitle() {
  QString title;
  if (!editingNewFile()) {
    title.append(QFileInfo(fileName_).fileName());
    if (fileIsModified()) {
      title.append("*");
    }
    title.append(" - ");
  }
  title.append(QCoreApplication::applicationName());
  setWindowTitle(title);
}

void MainWindow::updateCursorStatus() {
  QTextCursor cursor = ui_->editor->textCursor();
  int line = cursor.blockNumber() + 1;
  int column = cursor.columnNumber() + 1;
  dynamic_cast<StatusBar*>(statusBar())->setCursorPosition(line, column);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (closeFile()) {
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
        loadFile(url.toLocalFile());
      }
      break;
    }
  }
}

bool MainWindow::loadFile(QString fileName) {
  if (!fileName.isEmpty()) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
      QString message = tr("Could not open %1: %2.").arg(fileName)
                                                    .arg(file.errorString());
      QMessageBox::critical(this, QCoreApplication::applicationName(),
                            message, QMessageBox::Ok);
      return false;
    } else {
      fileName_ = fileName;
      ui_->editor->setPlainText(file.readAll());
      ui_->editor->document()->setModified(false);
      updateTitle();
      return true;
    }
  }
  return false;
}

bool MainWindow::editingNewFile() const {
  return fileName_.isEmpty();
}

bool MainWindow::fileIsModified() const {
  return ui_->editor->document()->isModified();
}

bool MainWindow::fileIsEmpty() const {
  QTextDocument *document = ui_->editor->document();
  return document->isEmpty() ||
         (editingNewFile() && !document->toPlainText().contains(QRegExp("\\S")));
}

bool MainWindow::fileIsCompletelyEmpty() const {
  return ui_->editor->document()->isEmpty();
}
