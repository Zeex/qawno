#include <QSettings>

#include "ReplaceDialog.h"
#include "ui_ReplaceDialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ReplaceDialog)
{
	ui->setupUi(this);
	ui->findWhatEdit->setFocus();

	// Load last state
	QSettings settings;
	settings.beginGroup("Widgets");
		settings.beginGroup("ReplaceDialog");
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

ReplaceDialog::~ReplaceDialog()
{
	// Save current state
	QSettings settings;
	settings.beginGroup("Widgets");
		settings.beginGroup("ReplaceDialog");
			settings.setValue("MatchCase", ui->matchCase->checkState() == Qt::Checked);
			settings.setValue("MatchWholeWords", ui->matchWholeWords->checkState() == Qt::Checked);
			settings.setValue("SearchBackwards", ui->searchBackwards->checkState() == Qt::Checked);
			settings.setValue("UseRegexp", ui->useRegexp->checkState() == Qt::Checked);
		settings.endGroup();
	settings.endGroup();

	delete ui;
}

QString ReplaceDialog::findWhatText() const
{
	return ui->findWhatEdit->text();
}

QString ReplaceDialog::replaceWithText() const
{
	return ui->replaceWithEdit->text();
}

bool ReplaceDialog::matchCase() const
{
	return ui->matchCase->checkState() == Qt::Checked;
}

bool ReplaceDialog::matchWholeWords() const
{
	return ui->matchWholeWords->checkState() == Qt::Checked;
}

bool ReplaceDialog::searchBackwards() const
{
	return ui->searchBackwards->checkState() == Qt::Checked;
}

bool ReplaceDialog::useRegexp() const
{
	return ui->useRegexp->checkState() == Qt::Checked;
}
