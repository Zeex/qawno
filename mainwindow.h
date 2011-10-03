#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QMenuBar;
class QPlainTextEdit;
class QProcess;
class QSplitter;
class QTextEdit;

class CodeEdit;
class PawnHighlighter;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);

protected:
	void closeEvent(QCloseEvent *closeEvent);

private slots:
	// File menu
	void newFile();
	void openFile();
	void closeFile();
	void saveFile();
	void saveFileAs();
	void exit();

	// Options menu
	void selectEditorFont();
	void selectOutputFont();

	// Build menu
	void compile();

	// Help menu
	void aboutQt();

	// Other
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
	// Code editor
	CodeEdit *m_codeEdit;
	PawnHighlighter *m_highlighter;

	// Currently edited file
	QString  m_fileName;

	// Compiler
	QString m_compilerPath;
	QProcess *m_compilerProcess;

	// Compiler output
	QPlainTextEdit *m_outputPane;

	// Splits editor and output area
	QSplitter *m_splitter;
};

#endif // MAINWINDOW_H
