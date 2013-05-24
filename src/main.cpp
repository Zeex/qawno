#include <QApplication>
#include <QCoreApplication>

#include "MainWindow.h"

int main(int argc, char **argv)
{
	QCoreApplication::setApplicationName("qawno");
	QCoreApplication::setApplicationVersion("1.0");
	QCoreApplication::setOrganizationName("SAMP");

	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}
