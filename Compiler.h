#ifndef COMPILER_H
#define COMPILER_H

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

class QProcess;
class QVariant;

class Compiler : public QObject
{
	Q_OBJECT
public:
	explicit Compiler(QObject *parent = 0);
	~Compiler();

	QString path() const;
	void setPath(const QString &path);

	QStringList options() const;
	void setOptions(const QString &options);
	void setOptions(const QStringList &options);

	QString getCommandLine(const QString &inputFile) const;

	bool test() const;
	void run(const QString &inputFile);

	QString getOutput() const;

signals:
	void finished(int);

private:
	QString m_path;
	QStringList m_options;
	QProcess *m_process;
};

#endif // COMPILER_H
