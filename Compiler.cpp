#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

#include "Compiler.h"

Compiler::Compiler(QObject *parent)
	: QObject(parent)
{
	m_process = new QProcess(this);
	m_process->setProcessChannelMode(QProcess::MergedChannels);
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

QStringList Compiler::options() const
{
	return m_options;
}

void Compiler::setOptions(const QString &options)
{
	m_options = options.split("\\s*");
}

void Compiler::setOptions(const QStringList &options)
{
	m_options = options;
}

bool Compiler::test() const
{
	m_process->start(m_path);
	m_process->waitForFinished();
	m_process->readAll();
	return m_process->error() != QProcess::FailedToStart;
}

void Compiler::run(const QString &inputFile)
{
	m_process->start(getCommandLine(inputFile), QProcess::ReadOnly);
}

QString Compiler::getCommandLine(const QString &inputFile) const
{
	return QString("%1 %2 \"%3\"").arg(m_path).arg(m_options.join(" ")).arg(inputFile);
}

QString Compiler::getOutput() const
{
	return m_process->readAll();
}
