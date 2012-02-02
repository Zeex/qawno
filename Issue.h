#ifndef ISSUE_H
#define ISSUE_H

#include <QObject>
#include <QString>

class Issue : public QObject
{
	Q_OBJECT

public:
	explicit Issue(const QString &message, QObject *parent = 0);

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

#endif // ISSUE_H
