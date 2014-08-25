// This file is part of qawno.
//
// qawno is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// qawno is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with qawno.  If not, see <http://www.gnu.org/licenses/>.

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
