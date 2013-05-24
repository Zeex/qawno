#include <QObject>
#include <QRegExp>
#include <QString>

#include "Issue.h"

Issue::Issue(const QString &message, QObject *parent)
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
