#include <QSettings>

#include "OutputWidget.h"

OutputWidget::OutputWidget(QWidget *parent): QPlainTextEdit(parent) {}

static QFont defaultFont() {
  #ifdef Q_OS_WINDOWS
    QFont font("Courier New");
  #else
    QFont font("Monospace");
  #endif
  font.setStyleHint(QFont::Monospace);
  return font;
}

OutputWidget::~OutputWidget() {
}

void OutputWidget::loadSettings() {
  QSettings settings;
  QFont font = defaultFont();
  font.fromString(settings.value("Font/Output", font).toString());
  QPlainTextEdit::setFont(font);
}

void OutputWidget::saveSettings() {
  QSettings settings;
  settings.setValue("Font/Output", font().toString());
}
