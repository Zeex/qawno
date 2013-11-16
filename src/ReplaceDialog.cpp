#include <QSettings>

#include "ReplaceDialog.h"
#include "ui_ReplaceDialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::ReplaceDialog)
{
  ui_->setupUi(this);
  ui_->findWhatEdit->setFocus();

  QSettings settings;
  settings.beginGroup("Widgets/ReplaceDialog");
    ui_->matchCase->setChecked(settings.value("MatchCase").toBool());
    ui_->matchWholeWords->setChecked(settings.value("MatchWholeWords").toBool());
    ui_->searchBackwards->setChecked(settings.value("SearchBackwards").toBool());
    ui_->useRegexp->setChecked(settings.value("UseRegexp").toBool());
  settings.endGroup();
}

ReplaceDialog::~ReplaceDialog() {
  QSettings settings;
  settings.beginGroup("Widgets/ReplaceDialog");
    settings.setValue("MatchCase", ui_->matchCase->isChecked());
    settings.setValue("MatchWholeWords", ui_->matchWholeWords->isChecked());
    settings.setValue("SearchBackwards", ui_->searchBackwards->isChecked());
    settings.setValue("UseRegexp", ui_->useRegexp->isChecked());
  settings.endGroup();
  delete ui_;
}

QString ReplaceDialog::findWhatText() const {
  return ui_->findWhatEdit->text();
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

bool ReplaceDialog::searchBackwards() const {
  return ui_->searchBackwards->isChecked();
}

bool ReplaceDialog::useRegexp() const {
  return ui_->useRegexp->isChecked();
}
