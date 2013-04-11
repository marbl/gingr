
#include <QtGui>
#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	
	MainWindow mainWindow(argc, argv);
//	mainWindow.show();
	
	return app.exec();
}
