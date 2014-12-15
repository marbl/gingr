// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include <QtGui>
#include "MainWindow.h"

int main(int argc, char **argv)
{
	qputenv("LIBOVERLAY_SCROLLBAR", 0);
	
	QApplication app(argc, argv);
	
	MainWindow mainWindow(argc, argv);
//	mainWindow.show();
	
	return app.exec();
}
