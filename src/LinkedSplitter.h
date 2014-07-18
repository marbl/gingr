// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__LinkedSplitter__
#define __gavqt__LinkedSplitter__

#include <QSplitter>

class LinkedSplitter : public QSplitter
{
	Q_OBJECT
	
public:
public slots:
	
	void moveSplitter(int pos, int index);
};

#endif /* defined(__gavqt__LinkedSplitter__) */
