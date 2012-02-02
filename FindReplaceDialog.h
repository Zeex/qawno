#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
	class FindReplaceDialog;
}

class FindReplaceDialog : public QDialog
{
	Q_OBJECT
public:
	explicit FindReplaceDialog(QWidget *parent = 0);
	~FindReplaceDialog();

	QString findWhatText() const;
	QString replaceWithText() const;

	bool matchCase() const;
	bool matchWholeWords() const;
	bool useRegexp() const;

private:
	Ui::FindReplaceDialog *ui;
};

#endif // FINDDIALOG_H
