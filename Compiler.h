#ifndef COMPILER_H
#define COMPILER_H

#include <QList>
#include <QObject>
#include <QString>

class QProcess;
class QVariant;

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
