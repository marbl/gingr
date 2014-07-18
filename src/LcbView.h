// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__LcbView__
#define __gavqt__LcbView__

#include "DrawingArea.h"
#include "Alignment.h"

class LcbView : public DrawingArea
{
public:
	
	void setAlignment(const Alignment * newAlignment);
	void setWindow(int newStart, int newEnd);
	
protected:
	
	void updateBuffer();
	
private:
	
	const Alignment * alignment;
	int start;
	int end;
};

#endif /* defined(__gavqt__LcbView__) */
