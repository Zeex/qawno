#include <QtGui>

#include "codeedit.h"
#include "mainwindow.h"
#include "pawnhighlighter.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_codeEdit = new CodeEdit(this);
	connect(m_codeEdit, SIGNAL(textChanged()), SLOT(updateWindowTitle()));

	m_highlighter = new PawnHighlighter(m_codeEdit);
	m_highlighter->setDocument(m_codeEdit->document());

	setMenuBar(new QMenuBar(this));

	QMenu *fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(tr("New"), this, SLOT(newFile()), QKeySequence("Ctrl+N"));
	fileMenu->addAction(tr("Open"), this, SLOT(openFile()), QKeySequence("Ctrl+O"));
	fileMenu->addAction(tr("Close"), this, SLOT(closeFile()), QKeySequence("Ctrl+X"));
	fileMenu->addSeparator();
	fileMenu->addAction(tr("Save"), this, SLOT(saveFile()), QKeySequence("Ctrl+S"));
	fileMenu->addAction(tr("Save as..."), this, SLOT(saveFileAs()), QKeySequence("Ctrl+Shift+S"));
	fileMenu->addSeparator();
	fileMenu->addAction(tr("Exit"), this, SLOT(exit()), QKeySequence("Ctrl+Q"));
	menuBar()->addMenu(fileMenu);

	QMenu *editMenu = new QMenu(tr("&Edit"), this);
	editMenu->addAction(tr("Undo"), m_codeEdit, SLOT(undo()), QKeySequence("Ctrl+Z"));
	editMenu->addAction(tr("Redo"), m_codeEdit, SLOT(undo()), QKeySequence("Ctrl+R"));
	editMenu->addSeparator();
	editMenu->addAction(tr("Cut"), m_codeEdit, SLOT(cut()), QKeySequence("Ctrl+X"));
	editMenu->addAction(tr("Copy"), m_codeEdit, SLOT(copy()), QKeySequence("Ctrl+C"));
	editMenu->addAction(tr("Paste"), m_codeEdit, SLOT(paste()), QKeySequence("Ctrl+V"));
	menuBar()->addMenu(editMenu);

	QMenu *buildMenu = new QMenu(tr("&Build"), this);
	buildMenu->addAction(tr("Compile"), this, SLOT(compile()), QKeySequence("F5"));
	menuBar()->addMenu(buildMenu);

	QMenu *optionsMenu = new QMenu(tr("&Options"), this);
	QMenu *fontMenu = optionsMenu->addMenu(tr("Font"));
	fontMenu->addAction(tr("Editor"), this, SLOT(selectEditorFont()));
	fontMenu->addAction(tr("Output Pane"), this, SLOT(selectOutputFont()));
	optionsMenu->addAction(tr("Compiler"), this, SLOT(setupCompiler()));
	menuBar()->addMenu(optionsMenu);

	QMenu *helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(tr("About Qt"), this, SLOT(aboutQt()));
	menuBar()->addMenu(helpMenu);

	m_outputPane = new QPlainTextEdit(this);
	m_outputPane->setReadOnly(true);

	m_splitter = new QSplitter(Qt::Vertical, this);
	m_splitter->addWidget(m_codeEdit);
	m_splitter->addWidget(m_outputPane);
	setCentralWidget(m_splitter);

	m_compilerProcess = new QProcess(this);
	connect(m_compilerProcess, SIGNAL(finished(int)), this, SLOT(compiled(int)));

	loadSettings();
}

void MainWindow::closeEvent(QCloseEvent *closeEvent)
{
	if (exit()) {
		saveSettings();
		closeEvent->accept();
	} else {
		// Exit cancelled by the user
		closeEvent->ignore();
	}
}

void MainWindow::newFile()
{
	closeFile();
}

void MainWindow::openFile()
{
	if (!closeFile()) {
		return;
	}

	QFileDialog openDialog(this);
	m_fileName = openDialog.getOpenFileName(this,
		tr("Open file"), "", tr("Pawn scripts (*.pwn *.inc)"));

	if (!m_fileName.isEmpty()) {
		QFile file(m_fileName);
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::critical(this, QCoreApplication::applicationName(),
				tr("Could not open %1: %2.")
					.arg(m_fileName)
					.arg(file.errorString()),
				QMessageBox::Ok);
			m_fileName.clear();
		} else {
			m_codeEdit->setPlainText(file.readAll());
			m_codeEdit->document()->setModified(false);
			updateWindowTitle();
		}
	}
}

bool MainWindow::closeFile()
{
	if (m_codeEdit->document()->isModified()
		&& !m_codeEdit->document()->isEmpty())
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
			if (!saveFile()) {
				return false;
			}
			break;
		case QMessageBox::No:
			break;
		case QMessageBox::Cancel:
			return false;
		}
	}

	m_codeEdit->clear();
	m_fileName.clear();

	return true;
}

bool MainWindow::saveFile()
{
	if (m_codeEdit->document()->isEmpty()) {
		return false;
	}

	if (m_fileName.isEmpty()) {
		return saveFileAs();
	}

	QFile file(m_fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this, QCoreApplication::applicationName(),
			tr("Could not save to %1: %2.")
				.arg(m_fileName)
				.arg(file.errorString()),
			QMessageBox::Ok);
		return false;
	}

	file.write(m_codeEdit->toPlainText().toAscii());
	m_codeEdit->document()->setModified(false);
	updateWindowTitle();

	return true;
}

bool MainWindow::saveFileAs()
{
	if (m_codeEdit->document()->isEmpty()) {
		return false;
	}

	QFileDialog saveDialog;
	QString fileName = saveDialog.getSaveFileName(this,
		tr("Save file as"), "", tr("Pawn scripts (*.pwn *.inc)"));

	if (!fileName.isEmpty()) {
		m_fileName = fileName;
		return saveFile();
	}

	return false;
}

bool MainWindow::exit()
{
	if (closeFile()) {
		close();
		return true;
	}
	return false;
}

void MainWindow::selectEditorFont()
{
	QFontDialog fontDialog(this);

	bool ok = false;
	QFont newFont = fontDialog.getFont(&ok, m_codeEdit->font(), this,
		tr("Select editor font"));

	if (ok) {
		m_codeEdit->setFont(newFont);
	}
}

void MainWindow::selectOutputFont()
{
	QFontDialog fontDialog(this);

	bool ok = false;
	QFont newFont = fontDialog.getFont(&ok, m_outputPane->font(), this,
		tr("Select output font"));

	if (ok) {
		m_outputPane->setFont(newFont);
	}
}

void MainWindow::compile()
{
	if (m_codeEdit->toPlainText().isEmpty()) {
		m_outputPane->appendPlainText(tr("Nothing to compile!"));
		return;
	}

	if (m_fileName.isEmpty()) {
		saveFileAs();
		return;
	}

	if (!QFile::exists(m_compilerPath)) {
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

	m_outputPane->clear();

	//QStringList arguments;
	//arguments << m_fileName;
	m_compilerProcess->start(QString("%1 %2").arg(m_compilerPath).arg(m_fileName),
		QProcess::ReadOnly);;
}

void MainWindow::compiled(int exitCode)
{
	if (exitCode != 0) {
		m_outputPane->appendPlainText(m_compilerProcess->readAllStandardError());
	}
	m_outputPane->appendPlainText(m_compilerProcess->readAllStandardOutput());
}

void MainWindow::setupCompiler()
{
	QFileDialog selectDialog;
	//selectDialog.setDefaultSuffix("pawncc.exe");
	QString path = selectDialog.getOpenFileName(this,
		tr("Specify Pawn compiler location"), "pawncc.exe",
		tr("Executable programs (*.exe)"));
	if (!path.isEmpty()) {
		m_compilerPath = path;
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
		if (m_codeEdit->document()->isModified()) {
			title.append("*");
		}
		title.append(" - ");
	}
	title.append(QCoreApplication::applicationName());
	setWindowTitle(title);
}

void MainWindow::loadSettings()
{
	QSettings settings;

	settings.beginGroup("Window");
		resize(settings.value("Size", QSize(640, 480)).toSize());
		move(settings.value("Pos").toPoint());
		if (settings.value("Maximized", false).toBool()) {
			setWindowState(Qt::WindowMaximized);
		}
		QList<int> sizes;
		sizes.push_back(settings.value("EditorHeight", height() * 0.75).toInt());
		sizes.push_back(settings.value("OutputHeight", height() * 0.25).toInt());
		m_splitter->setSizes(sizes);
	settings.endGroup();

	settings.beginGroup("Font");
		QFont font;
		settings.beginGroup("Editor");
			font.setFamily(settings.value("Family", "Courier New").toString());
			font.setPointSize(settings.value("PointSize", 10).toInt());
			font.setBold(settings.value("Bold", false).toBool());
			font.setBold(settings.value("Italic", false).toBool());
			m_codeEdit->setFont(font);
		settings.endGroup();
		settings.beginGroup("OutputPane");
			font.setFamily(settings.value("Family", "Courier New").toString());
			font.setPointSize(settings.value("PointSize", 10).toInt());
			font.setBold(settings.value("Bold", false).toBool());
			font.setBold(settings.value("Italic", false).toBool());
			m_outputPane->setFont(font);
		settings.endGroup();
	settings.endGroup();

	settings.beginGroup("Compiler");
		m_compilerPath = settings.value("Path").toString();
	settings.endGroup();
}

void MainWindow::saveSettings()
{
	QSettings settings;

	settings.beginGroup("Window");
		settings.setValue("Maximized", isMaximized());
		if (!isMaximized()) {
			settings.setValue("Size", size());
			settings.setValue("Pos", pos());
		}
		settings.setValue("EditorHeight", m_splitter->widget(0)->height());
		settings.setValue("OutputHeight", m_splitter->widget(1)->height());
	settings.endGroup();

	settings.beginGroup("Font");
		settings.beginGroup("Editor");
			settings.setValue("Family", m_codeEdit->font().family());
			settings.setValue("PointSize", m_codeEdit->font().pointSize());
			settings.setValue("Bold", m_codeEdit->font().bold());
			settings.setValue("Italic", m_codeEdit->font().italic());
		settings.endGroup();
		settings.beginGroup("OutputPane");
			settings.setValue("Family", m_outputPane->font().family());
			settings.setValue("PointSize", m_outputPane->font().pointSize());
			settings.setValue("Bold", m_outputPane->font().bold());
			settings.setValue("Italic", m_outputPane->font().italic());
		settings.endGroup();
	settings.endGroup();

	settings.beginGroup("Compiler");
		settings.setValue("Path", m_compilerPath);
	settings.endGroup();
}
