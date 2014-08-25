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

#include <QSettings>

#include "ReplaceDialog.h"
#include "ui_ReplaceDialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::ReplaceDialog)
{
  ui_->setupUi(this);
  ui_->replaceWhatEdit->setFocus();

  QSettings settings;
  ui_->matchCase->setChecked(settings.value("ReplaceMatchCase").toBool());
  ui_->matchWholeWords->setChecked(settings.value("ReplaceMatchWholeWords").toBool());
  ui_->searchBackwards->setChecked(settings.value("ReplaceSearchBackwards").toBool());
  ui_->useRegExp->setChecked(settings.value("ReplaceUseRegexp").toBool());
}

ReplaceDialog::~ReplaceDialog() {
  QSettings settings;
  settings.setValue("ReplaceMatchCase", ui_->matchCase->isChecked());
  settings.setValue("ReplaceMatchWholeWords", ui_->matchWholeWords->isChecked());
  settings.setValue("ReplaceSearchBackwards", ui_->searchBackwards->isChecked());
  settings.setValue("ReplaceUseRegexp", ui_->useRegExp->isChecked());
  delete ui_;
}

QString ReplaceDialog::replaceWhatText() const {
  return ui_->replaceWhatEdit->text();
}

QString ReplaceDialog::replaceWithText() const {
  return ui_->replaceWithEdit->text();
}

bool ReplaceDialog::matchCase() const {
  return ui_->matchCase->isChecked();
}

bool ReplaceDialog::matchWholeWords() const {
  return ui_->matchWholeWords->isChecked();
}

bool ReplaceDialog::useRegExp() const {
  return ui_->useRegExp->isChecked();
}

bool ReplaceDialog::searchBackwards() const {
  return ui_->searchBackwards->isChecked();
}
