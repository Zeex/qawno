#include "StatusBar.h"

StatusBar::StatusBar(QWidget *parent)
  : QStatusBar(parent)
{
}

void StatusBar::setCursorPosition(int line, int column) {
  showMessage(tr("Line %1, Column %2").arg(line).arg(column));
}
