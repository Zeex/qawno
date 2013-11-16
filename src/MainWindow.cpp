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
#include <QMimeData>
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
    ui_(new Ui::MainWindow)
{
  ui_->setupUi(this);

  connect(ui_->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
  connect(ui_->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(ui_->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
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
  connect(ui_->editor, SIGNAL(textChanged()), SLOT(refreshTitle()));
  connect(&compiler_, SIGNAL(finished(int)), this, SLOT(postCompile(int)));

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
    QFileDialog openDialog(this);
    QString fileName = openDialog.getOpenFileName(this, tr("Open file"), "",
                                              tr("Pawn scripts (*.pwn *.inc)"));
    loadFile(fileName);
    return true;
  }
  return false;
}

bool MainWindow::closeFile() {
  bool canClose = true;

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
    if (fileName_.isEmpty()) {
      return saveFileAs();
    } else {
      QFile file(fileName_);
      if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, QCoreApplication::applicationName(),
          tr("Could not save to %1: %2.")
          .arg(fileName_)
          .arg(file.errorString()),
          QMessageBox::Ok);
      } else {
        file.write(ui_->editor->toPlainText().toLatin1());
        ui_->editor->document()->setModified(false);
        refreshTitle();
      }
    }
  }
  return true;
}

bool MainWindow::saveFileAs() {
  if (!ui_->editor->document()->isEmpty()) {
    QFileDialog saveDialog;
    QString fileName = saveDialog.getSaveFileName(this,
                      tr("Save file as"), "", tr("Pawn scripts (*.pwn *.inc)"));
    if (!fileName.isEmpty()) {
      fileName_ = fileName;
      return saveFile();
    }
  }
  return false;
}

void MainWindow::find() {
  {
    FindDialog findDialog;
    findDialog.exec();
  }
  emit(findNext());
}

void MainWindow::findNext() {
  FindDialog findDialog(this);

  QTextDocument::FindFlags flags;
  if (findDialog.matchCase()) {
    flags |= QTextDocument::FindCaseSensitively;
  }
  if (findDialog.matchWholeWords()) {
    flags |= QTextDocument::FindWholeWords;
  }
  if (findDialog.searchBackwards()) {
    flags |= QTextDocument::FindBackward;
  }

  QTextCursor cursor = ui_->editor->textCursor();
  if (findDialog.useRegexp()) {
    Qt::CaseSensitivity caseSens = findDialog.matchCase()? Qt::CaseSensitive:
                                                           Qt::CaseInsensitive;
    QRegExp regexp(findDialog.findWhatText(), caseSens);
    cursor = ui_->editor->document()->find(regexp, cursor, flags);
  } else {
    cursor = ui_->editor->document()->find(findDialog.findWhatText(),
                                           cursor, flags);
  }

  if (!cursor.isNull()) {
    ui_->editor->setTextCursor(cursor);
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

void MainWindow::compile() {
  if (!compiler_.works()) {
    setupCompiler();
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

void MainWindow::postCompile(int /*exitCode*/) {
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

void MainWindow::refreshTitle() {
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
      refreshTitle();
      return true;
    }
  }
  return false;
}
