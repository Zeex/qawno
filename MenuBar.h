#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class QAction;

class MenuBar : public QMenuBar
{
	Q_OBJECT

public:
	struct Actions {
		QAction *fileNew;
		QAction *fileOpen;
		QAction *fileSave;
		QAction *fileSaveAs;
		QAction *fileClose;
		QAction *fileExit;
		QAction *editUndo;
		QAction *editRedo;
		QAction *editCut;
		QAction *editCopy;
		QAction *editPaste;
		QAction *editGoToLine;
		QAction *optionsFontEditor;
		QAction *optionsFontOutput;
		QAction *optionsCompiler;
		QAction *buildCompile;
		QAction *helpAboutQt;
	};

	explicit MenuBar(QWidget *parent = 0);

	const Actions &actions() const { return m_actions; }

private:
	Actions m_actions;
};

#endif // MENUBAR_H
