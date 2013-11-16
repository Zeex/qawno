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
  settings.beginGroup("Widgets/FindDialog");
    ui_->findWhatEdit->setText(settings.value("FindWhatText").toString());
    ui_->matchCase->setChecked(settings.value("MatchCase").toBool());
    ui_->matchWholeWords->setChecked(settings.value("MatchWholeWords").toBool());
    ui_->searchBackwards->setChecked(settings.value("SearchBackwards").toBool());
    ui_->useRegExp->setChecked(settings.value("UseRegexp").toBool());
  settings.endGroup();
}

FindDialog::~FindDialog() {
  QSettings settings;
  settings.beginGroup("Widgets/FindDialog");
    settings.setValue("FindWhatText", ui_->findWhatEdit->text());
    settings.setValue("MatchCase", ui_->matchCase->isChecked());
    settings.setValue("MatchWholeWords", ui_->matchWholeWords->isChecked());
    settings.setValue("SearchBackwards", ui_->searchBackwards->isChecked());
    settings.setValue("UseRegexp", ui_->useRegExp->isChecked());
  settings.endGroup();
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
