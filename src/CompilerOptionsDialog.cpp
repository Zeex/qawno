#include <QFileDialog>

#include "ui_CompilerOptionsDialog.h"
#include "CompilerOptionsDialog.h"

CompilerOptionsDialog::CompilerOptionsDialog(QWidget *parent)
  : QDialog(parent),
    ui_(new Ui::CompilerOptionsDialog)
{
  ui_->setupUi(this);
}

CompilerOptionsDialog::~CompilerOptionsDialog() {
  delete ui_;
}

QString CompilerOptionsDialog::getCompilerPath() const {
  return ui_->compilerExecutableEdit->text();
}

void CompilerOptionsDialog::setCompilerPath(const QString &path) {
  ui_->compilerExecutableEdit->setText(path);
}

QString CompilerOptionsDialog::getCompilerOptions() const {
  return ui_->commandLineEdit->text();
}

void CompilerOptionsDialog::setCompilerOptions(const QString &options) {
  ui_->commandLineEdit->setText(options);
}

void CompilerOptionsDialog::on_browseButton_clicked() {
  QString path = QFileDialog::getOpenFileName(this,
  #ifdef Q_WS_WIN
    tr("Select location of the Pawn compiler"), "pawncc.exe",
    tr("Executable programs (*.exe)"));
  #else
    tr("Select location of the Pawn compiler"), "pawncc",
    tr("All files (*.*)"));
  #endif
  if (!path.isEmpty()) {
    ui_->compilerExecutableEdit->setText(path);
  }
}
