#ifndef COMPILEROPTIONSDIALOG_H
#define COMPILEROPTIONSDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
  class CompilerOptionsDialog;
}

class CompilerOptionsDialog: public QDialog {
 Q_OBJECT

 public:
  explicit CompilerOptionsDialog(QWidget *parent = 0);
  ~CompilerOptionsDialog();

  QString compilerPath() const;
  void setCompilerPath(const QString &path);

  QString compilerOptions() const;
  void setCompilerOptions(const QString &options);

 private slots:
  void on_browseButton_clicked();

 private:
  Ui::CompilerOptionsDialog *ui_;
};

#endif // COMPILEROPTIONSDIALOG_H
