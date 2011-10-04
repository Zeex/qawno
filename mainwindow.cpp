#include <QApplication>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QMessageBox>
#include <QSettings>

#include "compiler.h"
#include "editor.h"
#include "issuelist.h"
#include "mainwindow.h"
#include "menubar.h"
#include "outputwidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_editor = new Editor(this);

	setCentralWidget(m_editor);
	connect(m_editor, SIGNAL(textChanged()), SLOT(updateWindowTitle()));

	MenuBar *menuBar = new MenuBar(this);
	setMenuBar(menuBar);
	connect(menuBar->actions().fileNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(menuBar->actions().fileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(menuBar->actions().fileClose, SIGNAL(triggered()), this, SLOT(closeFile()));
	connect(menuBar->actions().fileSave, SIGNAL(triggered()), this, SLOT(saveFile()));
	connect(menuBar->actions().fileSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));
	connect(menuBar->actions().fileExit, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(menuBar->actions().editUndo, SIGNAL(triggered()), m_editor, SLOT(undo()));
	connect(menuBar->actions().editUndo, SIGNAL(triggered()), m_editor, SLOT(redo()));
	connect(menuBar->actions().editCut, SIGNAL(triggered()), m_editor, SLOT(cut()));
	connect(menuBar->actions().editCopy, SIGNAL(triggered()), m_editor, SLOT(copy()));
	connect(menuBar->actions().editPaste, SIGNAL(triggered()), m_editor, SLOT(paste()));
	connect(menuBar->actions().buildCompile, SIGNAL(triggered()), this, SLOT(compile()));
	connect(menuBar->actions().optionsFontEditor, SIGNAL(triggered()), SLOT(selectEditorFont()));
	connect(menuBar->actions().optionsFontOutput, SIGNAL(triggered()), SLOT(selectOutputFont()));
	connect(menuBar->actions().helpAboutQt, SIGNAL(triggered()), SLOT(aboutQt()));

	//QDockWidget *issuesDock = new QDockWidget(tr("Issues"), this);
	//issuesDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	//m_issueList = new IssueList(this);
	//issuesDock->setWidget(m_issueList);
	//addDockWidget(Qt::BottomDockWidgetArea, issuesDock);

	QDockWidget *outputDock = new QDockWidget(tr("Output"), this);
	outputDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
	m_outputWidget = new OutputWidget(this);
	m_outputWidget->setReadOnly(true);
	outputDock->setWidget(m_outputWidget);
	addDockWidget(Qt::BottomDockWidgetArea, outputDock);

	//tabifyDockWidget(issuesDock, outputDock);

	m_compiler = new Compiler(this);
	m_compiler->setPath("pawncc.exe"); // it may be in current dir or in PATH
	connect(m_compiler, SIGNAL(finished(int)), this, SLOT(compiled(int)));

	// Restore window settings
	readSettings();

	// Open specified file, if any
	if (QApplication::instance()->argc() > 1) {
		readFile(QApplication::instance()->argv()[1]);
	}
}

void MainWindow::newFile()
{
	closeFile();
}

void MainWindow::openFile()
{
	if (!isSafeToClose()) {
		return;
	}

	m_fileName.clear();

	QFileDialog openDialog(this);
	QString fileName = openDialog.getOpenFileName(this,
		tr("Open file"), "", tr("Pawn scripts (*.pwn *.inc)"));

	readFile(fileName);
}

bool MainWindow::isSafeToClose()
{
	if (m_editor->document()->isModified()
		&& !m_editor->document()->isEmpty())
	{
		QString message;

		if (!m_fileName.isEmpty()) {
			message = tr("Save changes to %1?").arg(m_fileName);
		} else {
			message = tr("Save changes to a new file?");
		}

		int button = QMessageBox::question(this, QCoreApplication::applicationName(),
			message, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		switch (button) {
		case QMessageBox::Yes:
			saveFile();
			if (m_editor->document()->isModified()) {
				// Save file failed or cancelled
				return false;
			}
			return true;
		case QMessageBox::No:
			break;
		case QMessageBox::Cancel:
			return false;
		}
	}

	return true;
}

void MainWindow::closeFile()
{
	if (isSafeToClose()) {
		m_editor->clear();
		m_fileName.clear();
	}
}

void MainWindow::saveFile()
{
	if (m_editor->document()->isEmpty()) {
		return;
	}

	if (m_fileName.isEmpty()) {
		saveFileAs();
		return;
	}

	writeFile(m_fileName);
}

void MainWindow::saveFileAs()
{
	if (m_editor->document()->isEmpty()) {
		return;
	}

	QFileDialog saveDialog;
	QString fileName = saveDialog.getSaveFileName(this,
		tr("Save file as"), "", tr("Pawn scripts (*.pwn *.inc)"));

	if (!fileName.isEmpty()) {
		m_fileName = fileName;
		saveFile();
		return;
	}
}

void MainWindow::exit()
{
	if (isSafeToClose()) {
		close();
	}
}

void MainWindow::selectEditorFont()
{
	QFontDialog fontDialog(this);

	bool ok = false;
	QFont newFont = fontDialog.getFont(&ok, m_editor->font(), this,
		tr("Select editor font"));

	if (ok) {
		m_editor->setFont(newFont);
	}
}

void MainWindow::selectOutputFont()
{
	QFontDialog fontDialog(this);

	bool ok = false;
	QFont newFont = fontDialog.getFont(&ok, m_outputWidget->font(), this,
		tr("Select output font"));

	if (ok) {
		m_outputWidget->setFont(newFont);
	}
}

void MainWindow::compile()
{
	if (m_editor->toPlainText().isEmpty()) {
		m_outputWidget->appendPlainText(tr("Nothing to compile!"));
		return;
	}

	if (m_fileName.isEmpty()) {
		saveFileAs();
		return;
	}

	if (!QFile::exists(m_compiler->path())) {
		int button = QMessageBox::warning(this, QCoreApplication::applicationName(),
			tr("Compiler is not set or missing. \n\n"
				"You now will be asked to specify its location. "
				"The compiler usually sits inside the \"pawno\" folder "
				"of your SA:MP server installation.\n\n"
				"Continue?"),
			QMessageBox::Yes | QMessageBox::No);
		if (button != QMessageBox::No) {
			setupCompiler();
		}
		return;
	}

	m_compiler->run(m_fileName, "");
}

void MainWindow::compiled(int exitCode)
{
	QString output = m_compiler->output();
	m_outputWidget->setPlainText(output);

	// Remove old items
//	while (m_issueList->rowCount() != 0) {
//		m_issueList->removeRow(0);
//	}

//	QStringList lines = output.split('\n');
//	for (int i = 0; i < lines.size(); ++i) {
//		CompilerMessage item(lines.at(i));
//		if (item.isOk()) {
//			m_outputWidget->appendPlainText(" file: " + item.file() + '\n');
//			m_outputWidget->appendPlainText(" line: " + item.line() + '\n');
//			m_outputWidget->appendPlainText(" text: " + item.text() + '\n');
			//int row = m_problemList->rowCount();
//			m_issueList->insertRow(i);
//			m_issueList->setItem(i, 0, new QTableWidgetItem(item.file()));
//			m_issueList->setItem(i, 1, new QTableWidgetItem(item.line()));
//			m_issueList->setItem(i, 2, new QTableWidgetItem(item.text()));
//			if (item.type() == "fatal error") {
				//
//			} else if (item.type() == "error") {
				//
//			} else if (item.type() == "warning") {
				//
//			}
//		}
//	}
}

void MainWindow::setupCompiler()
{
	QFileDialog selectDialog;
	//selectDialog.setDefaultSuffix("pawncc.exe");
	QString path = selectDialog.getOpenFileName(this,
		tr("Specify Pawn compiler location"), "pawncc.exe",
		tr("Executable programs (*.exe)"));
	if (!path.isEmpty()) {
		m_compiler->setPath(path);
	}
}

void MainWindow::aboutQt()
{
	QMessageBox::aboutQt(this);
}

void MainWindow::updateWindowTitle()
{
	QString title;
	if (!m_fileName.isEmpty()) {
		title.append(QFileInfo(m_fileName).fileName());
		if (m_editor->document()->isModified()) {
			title.append("*");
		}
		title.append(" - ");
	}
	title.append(QCoreApplication::applicationName());
	setWindowTitle(title);
}

void MainWindow::closeEvent(QCloseEvent *closeEvent)
{
	if (isSafeToClose()) {
		writeSettings();
		closeEvent->accept();
	} else {
		closeEvent->ignore();
	}
}

void MainWindow::readFile(QString fileName)
{
	if (!fileName.isEmpty()) {
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::critical(this, QCoreApplication::applicationName(),
				tr("Could not open %1: %2.")
					.arg(fileName)
					.arg(file.errorString()),
				QMessageBox::Ok);
		} else {
			m_fileName = fileName;
			m_editor->setPlainText(file.readAll());
			m_editor->document()->setModified(false);
			updateWindowTitle();
		}
	}
}

void MainWindow::writeFile(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this, QCoreApplication::applicationName(),
			tr("Could not save to %1: %2.")
				.arg(fileName)
				.arg(file.errorString()),
			QMessageBox::Ok);
		return;
	}

	file.write(m_editor->toPlainText().toAscii());
	m_editor->document()->setModified(false);
	updateWindowTitle();
}

void MainWindow::readSettings()
{
	QSettings settings;

	settings.beginGroup("Window");
		resize(settings.value("Size", QSize(640, 480)).toSize());
		move(settings.value("Pos").toPoint());
		if (settings.value("Maximized", false).toBool()) {
			setWindowState(Qt::WindowMaximized);
		}
	settings.endGroup();

	settings.beginGroup("Compiler");
		m_compiler->setPath(settings.value("Path").toString());
	settings.endGroup();
}

void MainWindow::writeSettings()
{
	QSettings settings;

	settings.beginGroup("Window");
		settings.setValue("Maximized", isMaximized());
		if (!isMaximized()) {
			settings.setValue("Size", size());
			settings.setValue("Pos", pos());
		}
	settings.endGroup();

	settings.beginGroup("Compiler");
		settings.setValue("Path", m_compiler->path());
	settings.endGroup();
}
