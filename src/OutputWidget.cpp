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
  settings.beginGroup("UI");
    QFont font = defaultFont();
    font.fromString(settings.value("OutputFont", font).toString());
    QPlainTextEdit::setFont(font);
  settings.endGroup();
}

void OutputWidget::saveSettings() {
  QSettings settings;
  settings.beginGroup("UI");
    settings.setValue("OutputFont", font().toString());
  settings.endGroup();
}
