#include <cstdio>
#include <QProcess>
#include <QStringList>

#include "compiler.h"

CompilerMessage::CompilerMessage(const QString &message, QObject *parent)
	: QObject(parent)
	, m_ok(false)
{
	QRegExp re("(.*)\\((\\d*)\\) : \\b(warning|error|fatal error)\\b (\\d*): (.*)");
	int pos = re.indexIn(message);
	if (pos > -1) {
		m_file = re.cap(1);
		m_line = re.cap(2);
		m_type = re.cap(3);
		m_code = re.cap(4);
		m_text = re.cap(5);
		m_ok = true;
	}
}

Compiler::Compiler(QObject *parent)
	: QObject(parent)
{
	m_process = new QProcess(this);
	connect(m_process, SIGNAL(finished(int)), SIGNAL(finished(int)));
}

QString Compiler::path() const
{
	return m_path;
}

void Compiler::setPath(const QString &path)
{
	m_path = path;
}

bool Compiler::test() const
{
	m_process->start(QString("%1").arg(m_path));
	m_process->waitForFinished();
	return m_process->error() != QProcess::FailedToStart;
}

void Compiler::run(const QString &inputFile, const QString &options)
{
	m_process->start(QString("%1 %2 %3").arg(m_path).arg(inputFile).arg(options),
		QProcess::ReadOnly);
}

QString Compiler::output() const
{
	QString output;
	output.append(m_process->readAllStandardError());
	output.append(m_process->readAllStandardOutput());
	return output;
}
