//
//  LcbView.h
//  gavqt
//
//  Created by Brian Ondov on 4/9/13.
//
//

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
