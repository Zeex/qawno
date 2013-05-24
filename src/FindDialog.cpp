#include <QSettings>

#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::FindDialog)
{
  ui_->setupUi(this);
  ui_->findWhatEdit->setFocus();

  // Load last state
  QSettings settings;
  settings.beginGroup("Widgets");
    settings.beginGroup("FindDialog");
      ui_->matchCase->setCheckState(settings.value("MatchCase").toBool()
        ? Qt::Checked
        : Qt::Unchecked);
      ui_->matchWholeWords->setCheckState(settings.value("MatchWholeWords").toBool()
        ? Qt::Checked
        : Qt::Unchecked);
      ui_->searchBackwards->setCheckState(settings.value("SearchBackwards").toBool()
        ? Qt::Checked
        : Qt::Unchecked);
      ui_->useRegexp->setCheckState(settings.value("UseRegexp").toBool()
        ? Qt::Checked
        : Qt::Unchecked);
    settings.endGroup();
  settings.endGroup();
}

FindDialog::~FindDialog() {
  // Save current state
  QSettings settings;
  settings.beginGroup("Widgets");
    settings.beginGroup("FindDialog");
      settings.setValue("MatchCase", ui_->matchCase->checkState() == Qt::Checked);
      settings.setValue("MatchWholeWords", ui_->matchWholeWords->checkState() == Qt::Checked);
      settings.setValue("SearchBackwards", ui_->searchBackwards->checkState() == Qt::Checked);
      settings.setValue("UseRegexp", ui_->useRegexp->checkState() == Qt::Checked);
    settings.endGroup();
  settings.endGroup();

  delete ui_;
}

QString FindDialog::findWhatText() const {
  return ui_->findWhatEdit->text();
}

bool FindDialog::matchCase() const {
  return ui_->matchCase->checkState() == Qt::Checked;
}

bool FindDialog::matchWholeWords() const {
  return ui_->matchWholeWords->checkState() == Qt::Checked;
}

bool FindDialog::searchBackwards() const {
  return ui_->searchBackwards->checkState() == Qt::Checked;
}

bool FindDialog::useRegexp() const {
  return ui_->useRegexp->checkState() == Qt::Checked;
}
