//
//  StatusBar.cpp
//  gavqt
//
//  Created by Brian Ondov on 3/26/13.
//
//

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
