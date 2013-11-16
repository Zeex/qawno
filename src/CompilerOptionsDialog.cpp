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

QString CompilerOptionsDialog::compilerPath() const {
  return ui_->compilerExecutableEdit->text();
}

void CompilerOptionsDialog::setCompilerPath(const QString &path) {
  ui_->compilerExecutableEdit->setText(path);
}

QString CompilerOptionsDialog::compilerOptions() const {
  return ui_->commandLineEdit->text();
}

void CompilerOptionsDialog::setCompilerOptions(const QString &options) {
  ui_->commandLineEdit->setText(options);
}

void CompilerOptionsDialog::on_browseButton_clicked() {
  QString path = QFileDialog::getOpenFileName(this,
  #ifdef Q_WS_WIN
    tr("Set compiler executable"), "pawncc.exe",
    tr("Executables (*.exe)"));
  #else
    tr("Set compiler executable"), "pawncc",
    tr("All files (*.*)"));
  #endif
  if (!path.isEmpty()) {
    ui_->compilerExecutableEdit->setText(path);
  }
}
