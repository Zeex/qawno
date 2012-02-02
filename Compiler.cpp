#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

#include "Compiler.h"

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
