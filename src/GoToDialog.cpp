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

#include "GoToDialog.h"
#include "ui_GoToDialog.h"

GoToDialog::GoToDialog(QWidget *parent)
  : QDialog(parent),
    ui_(new Ui::GoToDialog)
{
  ui_->setupUi(this);
}

GoToDialog::~GoToDialog() {
  delete ui_;
}

int GoToDialog::targetLineNumber() const {
  return ui_->edit->text().toInt();
}
