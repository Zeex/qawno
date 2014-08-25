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

#ifndef COMPILEROPTIONSDIALOG_H
#define COMPILEROPTIONSDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
  class CompilerOptionsDialog;
}

class CompilerOptionsDialog: public QDialog {
 Q_OBJECT

 public:
  explicit CompilerOptionsDialog(QWidget *parent = 0);
  ~CompilerOptionsDialog() override;

  QString compilerPath() const;
  void setCompilerPath(const QString &path);

  QString compilerOptions() const;
  void setCompilerOptions(const QString &options);

 private slots:
  void on_browse_clicked();

private:
  Ui::CompilerOptionsDialog *ui_;
};

#endif // COMPILEROPTIONSDIALOG_H
