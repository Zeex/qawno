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
  return ui_->compilerPath->text();
}

void CompilerOptionsDialog::setCompilerPath(const QString &path) {
  ui_->compilerPath->setText(path);
}

QString CompilerOptionsDialog::compilerOptions() const {
  return ui_->compilerOptions->text();
}

void CompilerOptionsDialog::setCompilerOptions(const QString &options) {
  ui_->compilerOptions->setText(options);
}

void CompilerOptionsDialog::on_browse_clicked() {
  QString path = QFileDialog::getOpenFileName(this,
  #ifdef Q_OS_WIN
    tr("Set compiler executable"), "pawncc.exe",
    tr("Executables (*.exe)"));
  #else
    tr("Set compiler executable"), "pawncc",
    tr("All files (*.*)"));
  #endif
  if (!path.isEmpty()) {
    ui_->compilerPath->setText(path);
  }
}
