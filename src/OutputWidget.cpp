#include <QSettings>

#include "OutputWidget.h"

static QFont defaultFont() {
  #ifdef Q_OS_WINDOWS
    QFont font("Courier New");
  #else
    QFont font("Monospace");
  #endif
  font.setStyleHint(QFont::Monospace);
  return font;
}

OutputWidget::OutputWidget(QWidget *parent):
  QPlainTextEdit(parent)
{
  QSettings settings;
  QFont font = defaultFont();
  font.fromString(settings.value("OutputFont", font).toString());
  setFont(font);
}

OutputWidget::~OutputWidget() {
  QSettings settings;
  settings.setValue("OutputFont", font().toString());
}
