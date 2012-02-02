#ifndef ISSUELIST_H
#define ISSUELIST_H

#include <QTableWidget>

class IssueList : public QTableWidget
{
	Q_OBJECT

public:
	explicit IssueList(QWidget *parent = 0);
};

#endif // ISSUELIST_H
