#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QPlainTextEdit>

class OutputWidget: public QPlainTextEdit {
 Q_OBJECT

 public:
  explicit OutputWidget(QWidget *parent = 0);
  virtual ~OutputWidget();
};

#endif // OUTPUTWIDGET_H
