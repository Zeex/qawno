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
class Editor;
class IssueList;
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
	void aboutQt();
	void compiled(int);
	void setupCompiler();
	void updateWindowTitle();

private:
	bool isSafeToClose();
	void readFile(QString fileName);
	void writeFile(QString fileName);
	void readSettings();
	void writeSettings();

private:
	Editor *m_editor;
	QString  m_fileName; // Currently edited file

	Compiler *m_compiler;

	IssueList *m_issueList;
	OutputWidget *m_outputWidget;
};

#endif // MAINWINDOW_H
