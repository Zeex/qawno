#include <QSettings>

#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::FindDialog)
{
  ui_->setupUi(this);
  ui_->findWhatEdit->setFocus();

  QSettings settings;
  ui_->findWhatEdit->setText(settings.value("FindText").toString());
  ui_->matchCase->setChecked(settings.value("FindMatchCase").toBool());
  ui_->matchWholeWords->setChecked(settings.value("FindMatchWholeWords").toBool());
  ui_->searchBackwards->setChecked(settings.value("FindSearchBackwards").toBool());
  ui_->useRegExp->setChecked(settings.value("FindUseRegexp").toBool());
}

FindDialog::~FindDialog() {
  QSettings settings;
  settings.setValue("FindText", ui_->findWhatEdit->text());
  settings.setValue("FindMatchCase", ui_->matchCase->isChecked());
  settings.setValue("FindMatchWholeWords", ui_->matchWholeWords->isChecked());
  settings.setValue("FindSearchBackwards", ui_->searchBackwards->isChecked());
  settings.setValue("FindUseRegexp", ui_->useRegExp->isChecked());
  delete ui_;
}

QString FindDialog::findWhatText() const {
  return ui_->findWhatEdit->text();
}

bool FindDialog::matchCase() const {
  return ui_->matchCase->isChecked();
}

bool FindDialog::matchWholeWords() const {
  return ui_->matchWholeWords->isChecked();
}

bool FindDialog::useRegExp() const {
  return ui_->useRegExp->isChecked();
}

bool FindDialog::searchBackwards() const {
  return ui_->searchBackwards->isChecked();
}
