// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__StatusBar__
#define __gavqt__StatusBar__

#include <QLabel>

class StatusBar : public QLabel
{
	Q_OBJECT
	
public:
	
	StatusBar(QWidget * parent = 0);
	
	void setPosition(int gapped, int ungapped, int offset);
};

#endif /* defined(__gavqt__StatusBar__) */
