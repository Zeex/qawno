#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QMenuBar;
class QPlainTextEdit;
class QProcess;
class QSplitter;
class QTableWidget;
class QTextEdit;

class Compiler;
class EditorWidget;
//class IssueList;
class OutputWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);

protected:
	void closeEvent(QCloseEvent *closeEvent);

private slots:
	void newFile();
	void openFile();
	void closeFile();
	void saveFile();
	void saveFileAs();
	void exit();
	void selectEditorFont();
	void selectOutputFont();
	void compile();
	void setupCompiler();
	void aboutQt();
	void compiled(int);
	void updateWindowTitle();

private:
	bool isSafeToClose();

	void readFile(QString fileName);
	void writeFile(QString fileName);

	void readSettings();
	void writeSettings();	

private:	
	QString  m_fileName; // Currently edited file

	Compiler *m_compiler;

	EditorWidget *m_editor;
	OutputWidget *m_outputWidget;
	//IssueList *m_issueList;
};

#endif // MAINWINDOW_H
