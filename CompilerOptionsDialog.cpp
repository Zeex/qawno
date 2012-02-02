#include <QFileDialog>

#include "ui_CompilerOptionsDialog.h"
#include "CompilerOptionsDialog.h"

CompilerOptionsDialog::CompilerOptionsDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CompilerOptionsDialog)
{
	ui->setupUi(this);
}

CompilerOptionsDialog::~CompilerOptionsDialog()
{
	delete ui;
}

QString CompilerOptionsDialog::getCompilerPath() const
{
	return ui->compilerExecutableEdit->text();
}

void CompilerOptionsDialog::setCompilerPath(const QString &path)
{
	ui->compilerExecutableEdit->setText(path);
}

QString CompilerOptionsDialog::getCompilerOptions() const
{
	return ui->commandLineEdit->text();
}

void CompilerOptionsDialog::setCompilerOptions(const QString &options)
{
	ui->commandLineEdit->setText(options);
}

void CompilerOptionsDialog::on_browseButton_clicked()
{
	QString path = QFileDialog::getOpenFileName(this,
	#ifdef Q_WS_WIN
		tr("Select location of the Pawn compiler"), "pawncc.exe",
		tr("Executable programs (*.exe)"));
	#else
		tr("Select location of the Pawn compiler"), "pawncc",
		tr("All files (*.*)"));
	#endif
	if (!path.isEmpty()) {
		ui->compilerExecutableEdit->setText(path);
	}
}
