#ifndef COMPILER_H
#define COMPILER_H

#include <QList>
#include <QObject>
#include <QString>

class QProcess;
class QVariant;

class CompilerMessage : public QObject
{
	Q_OBJECT

public:
	explicit CompilerMessage(const QString &message, QObject *parent = 0);

	inline bool isOk() const { return m_ok; }

	inline const QString &file() const { return m_file; }
	inline const QString &line() const { return m_line; }
	inline const QString &type() const { return m_type; }
	inline const QString &code() const { return m_code; }
	inline const QString &text() const { return m_text; }

private:
	bool m_ok;
	QString m_file;
	QString m_line;
	QString m_type;
	QString m_code;
	QString m_text;
};

class Compiler : public QObject
{
	Q_OBJECT

public:
	explicit Compiler(QObject *parent = 0);

	QString path() const;
	void setPath(const QString &path);

	bool test() const;
	void run(const QString &inputFile, const QString &options = "");

	QString output() const;

signals:
	void finished(int);

private:
	QString m_path;
	QProcess *m_process;
};

#endif // COMPILER_H
