// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "StatusBar.h"
#include <QLabel>

StatusBar::StatusBar(QWidget * parent)
: QLabel(parent)
{
}

void StatusBar::setPosition(int gapped, int ungapped, int offset)
{
	if ( offset )
	{
		setText(QString("%1 + %2").arg(ungapped).arg(offset));
	}
	else
	{
		setText(QString("%1").arg(ungapped));
	}
}
