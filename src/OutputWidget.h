#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QPlainTextEdit>

class OutputWidget: public QPlainTextEdit {
 Q_OBJECT

 public:
  explicit OutputWidget(QWidget *parent = 0);

  virtual void setFont(const QFont &);
};

#endif // OUTPUTWIDGET_H
