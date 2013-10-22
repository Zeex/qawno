#include <QCoreApplication>

#include "MenuBar.h"

MenuBar::MenuBar(QWidget *parent)
  : QMenuBar(parent)
{
  QMenu *fileMenu = new QMenu(tr("&File"), this);
  actions_.fileNew = fileMenu->addAction(tr("New"));
  actions_.fileNew->setShortcut(QKeySequence("Ctrl+N"));
  actions_.fileOpen = fileMenu->addAction(tr("Open"));
  actions_.fileOpen->setShortcut(QKeySequence("Ctrl+O"));
  actions_.fileClose = fileMenu->addAction(tr("Close"));
  actions_.fileClose->setShortcut(QKeySequence("Ctrl+X"));
  fileMenu->addSeparator();
  actions_.fileSave = fileMenu->addAction(tr("Save"));
  actions_.fileSave->setShortcut(QKeySequence("Ctrl+S"));
  actions_.fileSaveAs = fileMenu->addAction(tr("Save as..."));
  actions_.fileSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
  fileMenu->addSeparator();
  actions_.fileExit = fileMenu->addAction(tr("Exit"));
  actions_.fileExit->setShortcut(QKeySequence("Ctrl+Q"));
  addMenu(fileMenu);

  QMenu *editMenu = new QMenu(tr("&Edit"), this);
  actions_.editUndo = editMenu->addAction(tr("Undo"));
  actions_.editUndo->setShortcut(QKeySequence("Ctrl+Z"));
  actions_.editRedo = editMenu->addAction(tr("Redo"));
  actions_.editRedo->setShortcut(QKeySequence("Ctrl+Y"));
  editMenu->addSeparator();
  actions_.editCut = editMenu->addAction(tr("Cut"));
  actions_.editCut->setShortcut(QKeySequence("Ctrl+X"));
  actions_.editCopy = editMenu->addAction(tr("Copy"));
  actions_.editCopy->setShortcut(QKeySequence("Ctrl+C"));
  actions_.editPaste = editMenu->addAction(tr("Paste"));
  actions_.editPaste->setShortcut(QKeySequence("Ctrl+V"));
  editMenu->addSeparator();
  actions_.editFind = editMenu->addAction(tr("Find..."));
  actions_.editFind->setShortcut(QKeySequence("Ctrl+F"));
  actions_.editFindNext = editMenu->addAction(tr("Find next"));
  actions_.editFindNext->setShortcut(QKeySequence("F3"));
  actions_.editReplace = editMenu->addAction(tr("Replace..."));
  actions_.editReplace->setShortcut(QKeySequence("Ctrl+H"));
  editMenu->addSeparator();
  actions_.editGoToLine = editMenu->addAction(tr("Go to line..."));
  actions_.editGoToLine->setShortcut(QKeySequence("Ctrl+G"));
  addMenu(editMenu);

  QMenu *buildMenu = new QMenu(tr("&Build"), this);
  actions_.buildCompile = buildMenu->addAction(tr("Compile"));
  actions_.buildCompile->setShortcut(QKeySequence("F5"));
  addMenu(buildMenu);

  QMenu *optionsMenu = new QMenu(tr("&Options"), this);
  QMenu *fontMenu = optionsMenu->addMenu(tr("Font"));
  actions_.optionsFontEditor = fontMenu->addAction(tr("Editor"));
  actions_.optionsFontOutput = fontMenu->addAction(tr("Output"));
  actions_.optionsCompiler = optionsMenu->addAction(tr("Compiler"));
  addMenu(optionsMenu);

  QMenu *helpMenu = new QMenu(tr("&Help"), this);
  QString appName = QCoreApplication::applicationName();
  actions_.helpAbout = helpMenu->addAction(tr("About %1...").arg(appName));
  actions_.helpAboutQt = helpMenu->addAction(tr("About Qt.."));
  addMenu(helpMenu);
}
