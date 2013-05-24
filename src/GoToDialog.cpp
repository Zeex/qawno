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
