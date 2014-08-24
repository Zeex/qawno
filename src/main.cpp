#include <QApplication>
#include <QCoreApplication>

#include <qawno.h>
#include "MainWindow.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QCoreApplication::setApplicationName("qawno");
  QCoreApplication::setApplicationVersion(QAWNO_VERSION_STRING);
  QCoreApplication::setOrganizationName("SAMP");

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
