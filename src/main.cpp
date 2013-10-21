#include <QApplication>
#include <QCoreApplication>

#include "MainWindow.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QCoreApplication::setApplicationName("qawno");
  QCoreApplication::setApplicationVersion("1.0");
  QCoreApplication::setOrganizationName("SA-MP");

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
