// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

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
	
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent * event);
	void updateBuffer();
	void wheelEvent(QWheelEvent * event);
	
signals:
	
	void positionChanged(int gapped);
	void signalMouseWheel(int delta);
	void signalWindowChanged(int start, int end);
	
private:
	
	void panToCursor();
	
	int start;
	int end;
	int top;
	int bottom;
	
	bool mouseDown;
};

#endif /* defined(__gavqt__BlockViewMap__) */
