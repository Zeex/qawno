// This file is part of qawno.
//
// qawno is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// qawno is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with qawno. If not, see <http://www.gnu.org/licenses/>.

#include <QFileDialog>

#include "ui_CompilerSettingsDialog.h"
#include "CompilerSettingsDialog.h"

CompilerSettingsDialog::CompilerSettingsDialog(QWidget *parent)
  : QDialog(parent),
    ui_(new Ui::CompilerSettingsDialog)
{
  ui_->setupUi(this);
}

CompilerSettingsDialog::~CompilerSettingsDialog() {
  delete ui_;
}

QString CompilerSettingsDialog::compilerPath() const {
  return ui_->compilerPath->text();
}

void CompilerSettingsDialog::setCompilerPath(const QString &path) {
  ui_->compilerPath->setText(path);
}

QString CompilerSettingsDialog::compilerOptions() const {
  return ui_->compilerOptions->text();
}

void CompilerSettingsDialog::setCompilerOptions(const QString &options) {
  ui_->compilerOptions->setText(options);
}

void CompilerSettingsDialog::on_browse_clicked() {
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
