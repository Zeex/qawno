#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
  : QDialog(parent),
    ui_(new Ui::AboutDialog)
{
  ui_->setupUi(this);
}

AboutDialog::~AboutDialog() {
  delete ui_;
}
