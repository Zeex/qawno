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

#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
  class ReplaceDialog;
}

class ReplaceDialog: public QDialog {
 Q_OBJECT

 public:
  explicit ReplaceDialog(QWidget *parent = 0);
  ~ReplaceDialog() override;

  QString replaceWhatText() const;
  QString replaceWithText() const;

  bool matchCase() const;
  bool matchWholeWords() const;
  bool searchBackwards() const;
  bool useRegExp() const;

 private:
  Ui::ReplaceDialog *ui_;
};

#endif // FINDDIALOG_H
