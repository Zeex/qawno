#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStringList>

#include "Compiler.h"

Compiler::Compiler(QObject *parent)
	: QObject(parent)
{
	m_process = new QProcess(this);
	m_process->setProcessChannelMode(QProcess::MergedChannels);
	connect(m_process, SIGNAL(finished(int)), SIGNAL(finished(int)));

	QSettings settings;
	settings.beginGroup("Compiler");
		m_path = settings.value("Path").toString();
		if (path().isEmpty()) {
			m_path = "pawncc"; // Assume compiler is in PATH
		}
		m_options = settings.value("Options").toString().split("\\s*");
	settings.endGroup();
}

Compiler::~Compiler()
{
	QSettings settings;
	settings.beginGroup("Compiler");
		settings.setValue("Path", m_path);
		settings.setValue("Options", m_options.join(" "));
	settings.endGroup();
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
	QProcess pawncc;
	pawncc.setProcessChannelMode(QProcess::SeparateChannels);
	pawncc.start(m_path);
	pawncc.waitForFinished();
	return pawncc.error() != QProcess::FailedToStart;
}

void Compiler::run(const QString &inputFile)
{
	m_process->start(getCommandLine(inputFile), QProcess::ReadOnly);
}

QString Compiler::getCommandLine(const QString &inputFile) const
{
	QFileInfo in(inputFile);
	QFileInfo out(in.absolutePath() + "/" + in.baseName() + ".amx");
	return QString("%1 %2 \"%3\" -o\"%4\"").arg(m_path).arg(m_options.join(" ")).arg(inputFile).arg(out.filePath());
}

QString Compiler::getOutput() const
{
	return m_process->readAll();
}
