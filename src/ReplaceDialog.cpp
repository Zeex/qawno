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
