#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QPlainTextEdit>

class OutputWidget: public QPlainTextEdit {
 Q_OBJECT

 public:
  explicit OutputWidget(QWidget *parent = 0);

 public slots:
  void loadSettings();
  void saveSettings();
};

#endif // OUTPUTWIDGET_H
