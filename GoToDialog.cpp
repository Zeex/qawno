#include "GoToDialog.h"
#include "ui_GoToDialog.h"

GoToDialog::GoToDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToDialog)
{
	ui->setupUi(this);
}

GoToDialog::~GoToDialog()
{
	delete ui;
}

long GoToDialog::getEnteredNumber() const
{
	return ui->edit->text().toLong();
}
