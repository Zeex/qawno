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

#include <QAction>
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

  bool useTabs = (ui_->editor->indentPolicy() == EditorWidget::IndentWithTabs);
  ui_->actionUseTabs->setChecked(useTabs);
  ui_->actionUseSpaces->setChecked(!useTabs);

  int tabWidth = ui_->editor->tabWidth();
  ui_->actionTabWidth2->setChecked(tabWidth == 2);
  ui_->actionTabWidth4->setChecked(tabWidth == 4);
  ui_->actionTabWidth8->setChecked(tabWidth == 8);

  int indentWidth = ui_->editor->indentWidth();
  ui_->actionIndentWidth2->setChecked(indentWidth == 2);
  ui_->actionIndentWidth4->setChecked(indentWidth == 4);
  ui_->actionIndentWidth8->setChecked(indentWidth == 8);

  QSettings settings;

  resize(settings.value("WindowSize", QSize(800, 600)).toSize());
  if (settings.value("Maximized", false).toBool()) {
    setWindowState(Qt::WindowMaximized);
  }

  if (QApplication::instance()->arguments().size() > 1) {
    loadFile(QApplication::instance()->arguments()[1]);
  }
}

MainWindow::~MainWindow() {
  QSettings settings;

  settings.setValue("Maximized", isMaximized());
  if (!isMaximized()) {
    settings.setValue("WindowSize", size());
  }

  delete ui_;
}

void MainWindow::on_actionNew_triggered() {
  emit on_actionClose_triggered();
}

void MainWindow::on_actionOpen_triggered() {
  on_actionClose_triggered();

  if (!isFileEmpty()) {
    return;
  }

  QSettings settings;
  QString dir = settings.value("LastOpenDir", "").toString();

  QString caption = tr("Open file");
  QString filter = tr("Pawn scripts (*.pwn *.inc)");
  QString fileName = QFileDialog::getOpenFileName(this, caption, dir, filter);

  if (!loadFile(fileName)) {
    return;
  }

  dir = QFileInfo(fileName).dir().path();
  settings.setValue("LastOpenDir", dir);
}

void MainWindow::on_actionClose_triggered() {
  bool canClose = true;

  if (isFileModified() && !isFileEmpty()) {
    QString message = (!isNewFile())
      ? tr("Save changes to %1?").arg(fileName_)
      : tr("Save changes to a new file?");
    int result = QMessageBox::question(this,
                                       QCoreApplication::applicationName(),
                                       message,
                                       QMessageBox::Yes | QMessageBox::No
                                                        | QMessageBox::Cancel);
    switch (result) {
      case QMessageBox::Yes:
        on_actionSave_triggered();
        canClose = !isFileModified();
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
}

void MainWindow::on_actionSave_triggered() {
  if (isNewFile()) {
    on_actionSaveAs_triggered();
    return;
  }

  QFile file(fileName_);
  if (!file.open(QIODevice::WriteOnly)) {
    QString message =
      tr("Could not save to %1: %2.").arg(fileName_, file.errorString());
    QMessageBox::critical(this,
                          QCoreApplication::applicationName(),
                          message,
                          QMessageBox::Ok);
    return;
  }

  file.write(ui_->editor->toPlainText().toLatin1());
  ui_->editor->textChanged();
  ui_->editor->document()->setModified(false);
}

void MainWindow::on_actionSaveAs_triggered() {
  QSettings settings;
  QString dir = settings.value("LastSaveDir").toString();

  QString caption = tr("Save file as");
  QString filter = tr("Pawn scripts (*.pwn *.inc)");
  QString fileName = QFileDialog::getSaveFileName(this, caption, dir, filter);

  if (fileName.isEmpty()) {
    return;
  }

  dir = QFileInfo(fileName).dir().path();
  settings.setValue("LastSaveDir", dir);

  fileName_ = fileName;
  return on_actionSave_triggered();
}

void MainWindow::on_actionFind_triggered() {
  bool found = false;
  {
    FindDialog dialog;
    dialog.exec();
    found = dialog.result() == QDialog::Accepted;
  }
  if (found) {
    findStart_ = ui_->editor->textCursor().position();
    findRound_ = 0;
    on_actionFindNext_triggered();
  }
}

void MainWindow::on_actionFindNext_triggered() {
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

  bool found = !next.isNull() &&
    (findRound_ == 0 || next.position() < findStart_);

  if (!found && findRound_ > 0) {
    QString string = dialog.findWhatText();
    QString message = tr("No matching text found for \"%1\".").arg(string);
    QMessageBox::information(this,
                              QCoreApplication::applicationName(),
                              message,
                              QMessageBox::Ok);
    findStart_ = next.position();
    findRound_ = 0;
    return;
  }

  if (!found && findRound_ == 0) {
    next = current;
    if (dialog.searchBackwards()) {
      next.movePosition(QTextCursor::End);
    } else {
      next.movePosition(QTextCursor::Start);
    }
  }

  ui_->editor->setTextCursor(next);

  if (!found && findRound_ == 0) {
    findRound_++;
    on_actionFindNext_triggered();
  }
}

void MainWindow::on_actionGoToLine_triggered() {
  GoToDialog dialog;
  dialog.exec();
  ui_->editor->jumpToLine(dialog.targetLineNumber());
}

void MainWindow::on_actionUseTabs_triggered() {
  ui_->editor->setIndentPolicy(EditorWidget::IndentWithTabs);
  ui_->actionUseTabs->setChecked(true);
  ui_->actionUseSpaces->setChecked(false);
}

void MainWindow::on_actionUseSpaces_triggered() {
  ui_->editor->setIndentPolicy(EditorWidget::IndentWithSpaces);
  ui_->actionUseTabs->setChecked(false);
  ui_->actionUseSpaces->setChecked(true);
}

void MainWindow::on_actionTabWidth2_triggered() {
  ui_->editor->setTabWidth(2);
  ui_->actionTabWidth2->setChecked(true);
  ui_->actionTabWidth4->setChecked(false);
  ui_->actionTabWidth8->setChecked(false);
}

void MainWindow::on_actionTabWidth4_triggered() {
  ui_->editor->setTabWidth(4);
  ui_->actionTabWidth2->setChecked(false);
  ui_->actionTabWidth4->setChecked(true);
  ui_->actionTabWidth8->setChecked(false);
}

void MainWindow::on_actionTabWidth8_triggered() {
  ui_->editor->setTabWidth(8);
  ui_->actionTabWidth2->setChecked(false);
  ui_->actionTabWidth4->setChecked(false);
  ui_->actionTabWidth8->setChecked(true);
}

void MainWindow::on_actionIndentWidth2_triggered() {
  ui_->editor->setIndentWidth(2);
  ui_->actionIndentWidth2->setChecked(true);
  ui_->actionIndentWidth4->setChecked(false);
  ui_->actionIndentWidth8->setChecked(false);
}

void MainWindow::on_actionIndentWidth4_triggered() {
  ui_->editor->setIndentWidth(4);
  ui_->actionIndentWidth2->setChecked(false);
  ui_->actionIndentWidth4->setChecked(true);
  ui_->actionIndentWidth8->setChecked(false);
}

void MainWindow::on_actionIndentWidth8_triggered() {
  ui_->editor->setIndentWidth(8);
  ui_->actionIndentWidth2->setChecked(false);
  ui_->actionIndentWidth4->setChecked(false);
  ui_->actionIndentWidth8->setChecked(true);
}

void MainWindow::on_actionEditorFont_triggered() {
  QFontDialog fontDialog(this);

  bool ok = false;
  QFont newFont = fontDialog.getFont(&ok, ui_->editor->font(), this,
                                     tr("Select editor font"));

  if (ok) {
    ui_->editor->setFont(newFont);
  }
}

void MainWindow::on_actionOutputFont_triggered() {
  QFontDialog fontDialog(this);

  bool ok = false;
  QFont newFont = fontDialog.getFont(&ok, ui_->output->font(), this,
  tr("Select output font"));

  if (ok) {
    ui_->output->setFont(newFont);
  }
}

void MainWindow::on_actionCompiler_triggered() {
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

void MainWindow::on_actionCompile_triggered() {
  if (isNewFile()) {
    on_actionSaveAs_triggered();
  } else {
    Compiler compiler;
    ui_->output->clear();
    ui_->output->appendPlainText(compiler.commandFor(fileName_));
    ui_->output->appendPlainText("\n");
    compiler.run(fileName_);
    ui_->output->appendPlainText(compiler.output());
  }
}

void MainWindow::on_actionAbout_triggered() {
  AboutDialog dialog;
  dialog.exec();
}

void MainWindow::on_actionAboutQt_triggered() {
  QMessageBox::aboutQt(this);
}

void MainWindow::on_editor_textChanged() {
  QString title;

  if (isNewFile()) {
    title = "New File";
  } else {
    title = QFileInfo(fileName_).fileName();
    if (isFileModified()) {
      title.append("*");
    }
  }

  title.append(" - ");
  title.append(QCoreApplication::applicationName());

  setWindowTitle(title);
}

void MainWindow::on_editor_cursorPositionChanged() {
  QTextCursor cursor = ui_->editor->textCursor();
  int line = cursor.blockNumber() + 1;
  int column = cursor.columnNumber() + 1;
  dynamic_cast<StatusBar*>(statusBar())->setCursorPosition(line, column);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  on_actionClose_triggered();
  if (isFileEmpty()) {
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
      on_actionClose_triggered();
      if (isFileEmpty()) {
        loadFile(url.toLocalFile());
      }
      break;
    }
  }
}

bool MainWindow::loadFile(const QString &fileName) {
  if (fileName.isEmpty()) {
    return false;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    QString message = tr("Could not open %1: %2.").arg(fileName)
                                                  .arg(file.errorString());
    QMessageBox::critical(this,
                          QCoreApplication::applicationName(),
                          message,
                          QMessageBox::Ok);
    return false;
  }

  fileName_ = fileName;
  ui_->editor->setPlainText(file.readAll());
  ui_->editor->document()->setModified(false);
  return true;
}

bool MainWindow::isNewFile() const {
  return fileName_.isEmpty();
}

bool MainWindow::isFileModified() const {
  return ui_->editor->document()->isModified();
}

bool MainWindow::isFileEmpty() const {
  QTextDocument *document = ui_->editor->document();
  return document->isEmpty()
    || ui_->editor->document()->isEmpty()
    || (isNewFile() && !document->toPlainText().contains(QRegExp("\\S")));
}
