//
//  BlockViewMap.h
//  gavqt
//
//  Created by Brian Ondov on 4/2/13.
//
//

#ifndef __gavqt__BlockViewMap__
#define __gavqt__BlockViewMap__

#include "BlockView.h"

class BlockViewMap : public BlockView
{
	Q_OBJECT
	
public:
	
	void setTrackZoom(int start, int end);
	void setWindow(int newStart, int newEnd);
	
protected:
	
	void paintEvent(QPaintEvent * event);
	
private:
	
	int start;
	int end;
	int top;
	int bottom;
};

#endif /* defined(__gavqt__BlockViewMap__) */
