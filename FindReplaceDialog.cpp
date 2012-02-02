#include "FindReplaceDialog.h"
#include "ui_FindReplaceDialog.h"

FindReplaceDialog::FindReplaceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FindReplaceDialog)
{
	ui->setupUi(this);
	ui->findWhatEdit->setFocus();
}

FindReplaceDialog::~FindReplaceDialog()
{
	delete ui;
}

QString FindReplaceDialog::findWhatText() const
{
	return ui->findWhatEdit->text();
}

QString FindReplaceDialog::replaceWithText() const
{
	return ui->replaceWithEdit->text();
}

bool FindReplaceDialog::matchCase() const
{
	return ui->matchCase->checkState() == Qt::Checked;
}

bool FindReplaceDialog::matchWholeWords() const
{
	return ui->matchWholeWords->checkState() == Qt::Checked;
}

bool FindReplaceDialog::searchBackwards() const
{
	return ui->searchBackwards->checkState() == Qt::Checked;
}

bool FindReplaceDialog::useRegexp() const
{
	return ui->useRegexp->checkState() == Qt::Checked;
}
