#include <QSettings>

#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FindDialog)
{
	ui->setupUi(this);
	ui->findWhatEdit->setFocus();

	// Load last state
	QSettings settings;
	settings.beginGroup("Widgets");
		settings.beginGroup("FindDialog");
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

FindDialog::~FindDialog()
{
	// Save current state
	QSettings settings;
	settings.beginGroup("Widgets");
		settings.beginGroup("FindDialog");
			settings.setValue("MatchCase", ui->matchCase->checkState() == Qt::Checked);
			settings.setValue("MatchWholeWords", ui->matchWholeWords->checkState() == Qt::Checked);
			settings.setValue("SearchBackwards", ui->searchBackwards->checkState() == Qt::Checked);
			settings.setValue("UseRegexp", ui->useRegexp->checkState() == Qt::Checked);
		settings.endGroup();
	settings.endGroup();

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
