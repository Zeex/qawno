#include <QHeaderView>

#include "IssueList.h"

IssueList::IssueList(QWidget *parent) :
	QTableWidget(parent)
{
	setRowCount(0);
	setColumnCount(3);

	verticalHeader()->hide();
	//m_problemList->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setResizeMode(QHeaderView::Interactive);

	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setHorizontalHeaderItem(0, new QTableWidgetItem(tr("File")));
	setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Line")));
	setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Details")));
}
