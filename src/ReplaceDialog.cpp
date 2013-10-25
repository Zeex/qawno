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
  settings.beginGroup("Widgets");
    settings.beginGroup("ReplaceDialog");
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

ReplaceDialog::~ReplaceDialog() {
  QSettings settings;
  settings.beginGroup("Widgets");
    settings.beginGroup("ReplaceDialog");
      settings.setValue("MatchCase", ui_->matchCase->checkState() == Qt::Checked);
      settings.setValue("MatchWholeWords", ui_->matchWholeWords->checkState() == Qt::Checked);
      settings.setValue("SearchBackwards", ui_->searchBackwards->checkState() == Qt::Checked);
      settings.setValue("UseRegexp", ui_->useRegexp->checkState() == Qt::Checked);
    settings.endGroup();
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
  return ui_->matchCase->checkState() == Qt::Checked;
}

bool ReplaceDialog::matchWholeWords() const {
  return ui_->matchWholeWords->checkState() == Qt::Checked;
}

bool ReplaceDialog::searchBackwards() const {
  return ui_->searchBackwards->checkState() == Qt::Checked;
}

bool ReplaceDialog::useRegexp() const {
  return ui_->useRegexp->checkState() == Qt::Checked;
}
