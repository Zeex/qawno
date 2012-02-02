#include "MenuBar.h"

MenuBar::MenuBar(QWidget *parent) :
	QMenuBar(parent)
{
	QMenu *fileMenu = new QMenu(tr("&File"), this);
	m_actions.fileNew = fileMenu->addAction(tr("New"));
	m_actions.fileNew->setShortcut(QKeySequence("Ctrl+N"));
	m_actions.fileOpen = fileMenu->addAction(tr("Open"));
	m_actions.fileOpen->setShortcut(QKeySequence("Ctrl+O"));
	m_actions.fileClose = fileMenu->addAction(tr("Close"));
	m_actions.fileClose->setShortcut(QKeySequence("Ctrl+X"));
	fileMenu->addSeparator();
	m_actions.fileSave = fileMenu->addAction(tr("Save"));
	m_actions.fileSave->setShortcut(QKeySequence("Ctrl+S"));
	m_actions.fileSaveAs = fileMenu->addAction(tr("Save as..."));
	m_actions.fileSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
	fileMenu->addSeparator();
	m_actions.fileExit = fileMenu->addAction(tr("Exit"));
	m_actions.fileExit->setShortcut(QKeySequence("Ctrl+Q"));
	addMenu(fileMenu);

	QMenu *editMenu = new QMenu(tr("&Edit"), this);
	m_actions.editUndo = editMenu->addAction(tr("Undo"));
	m_actions.editRedo = editMenu->addAction(tr("Redo"));
	editMenu->addSeparator();
	m_actions.editCut = editMenu->addAction(tr("Cut"));
	m_actions.editCopy = editMenu->addAction(tr("Copy"));
	m_actions.editPaste = editMenu->addAction(tr("Paste"));
	editMenu->addSeparator();
	m_actions.editGoToLine = editMenu->addAction(tr("Go to line..."));
	m_actions.editGoToLine->setShortcut(QKeySequence("Ctrl+G"));
	addMenu(editMenu);

	QMenu *buildMenu = new QMenu(tr("&Build"), this);
	m_actions.buildCompile = buildMenu->addAction(tr("Compile"));
	m_actions.buildCompile->setShortcut(QKeySequence("F5"));
	addMenu(buildMenu);

	QMenu *optionsMenu = new QMenu(tr("&Options"), this);
	QMenu *fontMenu = optionsMenu->addMenu(tr("Font"));
	m_actions.optionsFontEditor = fontMenu->addAction(tr("Editor"));
	m_actions.optionsFontOutput = fontMenu->addAction(tr("Output"));
	m_actions.optionsCompiler = optionsMenu->addAction(tr("Compiler"));
	addMenu(optionsMenu);

	QMenu *helpMenu = new QMenu(tr("&Help"), this);
	m_actions.helpAboutQt = helpMenu->addAction(tr("About Qt"), this, SLOT(aboutQt()));
	addMenu(helpMenu);
}
