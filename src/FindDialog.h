#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
	class FindDialog;
}

class FindDialog : public QDialog
{
	Q_OBJECT
public:
	explicit FindDialog(QWidget *parent = 0);
	~FindDialog();

	QString findWhatText() const;

	bool matchCase() const;
	bool matchWholeWords() const;
	bool searchBackwards() const;
	bool useRegexp() const;

private:
	Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
