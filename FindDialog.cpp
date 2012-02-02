#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
}

FindDialog::~FindDialog()
{
    delete ui;
}

QString FindDialog::findWhatText() const
{
	return ui->findWhatEdit->text();
}

bool FindDialog::matchCase() const
{
	return ui->matchCase->checkState() == Qt::Checked;
}

bool FindDialog::matchWholeWords() const
{
	return ui->matchWholeWords->checkState() == Qt::Checked;
}

bool FindDialog::searchBackwards() const
{
	return ui->searchBackwards->checkState() == Qt::Checked;
}

bool FindDialog::useRegexp() const
{
	return ui->useRegexp->checkState() == Qt::Checked;
}
