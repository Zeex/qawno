#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
  class ReplaceDialog;
}

class ReplaceDialog: public QDialog {
 Q_OBJECT

 public:
  explicit ReplaceDialog(QWidget *parent = 0);
  ~ReplaceDialog();

  QString findWhatText() const;
  QString replaceWithText() const;

  bool matchCase() const;
  bool matchWholeWords() const;
  bool searchBackwards() const;
  bool useRegexp() const;

 private:
  Ui::ReplaceDialog *ui_;
};

#endif // FINDDIALOG_H
