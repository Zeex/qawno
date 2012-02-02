#include <QSettings>

#include "FindReplaceDialog.h"
#include "ui_FindReplaceDialog.h"

FindReplaceDialog::FindReplaceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FindReplaceDialog)
{
	ui->setupUi(this);
	ui->findWhatEdit->setFocus();

	// Load last state
	QSettings settings;
	settings.beginGroup("Widgets");
		settings.beginGroup("FindReplaceDialog");
			ui->matchCase->setCheckState(settings.value("MatchCase").toBool()
				? Qt::Checked
				: Qt::Unchecked);
			ui->matchWholeWords->setCheckState(settings.value("MatchWholeWords").toBool()
				? Qt::Checked
				: Qt::Unchecked);
			ui->searchBackwards->setCheckState(settings.value("SearchBackwards").toBool()
				? Qt::Checked
				: Qt::Unchecked);
			ui->useRegexp->setCheckState(settings.value("UseRegexp").toBool()
				? Qt::Checked
				: Qt::Unchecked);
		settings.endGroup();
	settings.endGroup();
}

FindReplaceDialog::~FindReplaceDialog()
{
	// Save current state
	QSettings settings;
	settings.beginGroup("Widgets");
		settings.beginGroup("FindReplaceDialog");
			settings.setValue("MatchCase", ui->matchCase->checkState() == Qt::Checked);
			settings.setValue("MatchWholeWords", ui->matchWholeWords->checkState() == Qt::Checked);
			settings.setValue("SearchBackwards", ui->searchBackwards->checkState() == Qt::Checked);
			settings.setValue("UseRegexp", ui->useRegexp->checkState() == Qt::Checked);
		settings.endGroup();
	settings.endGroup();

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
