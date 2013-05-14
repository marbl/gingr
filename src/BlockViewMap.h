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
	
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent * event);
	void wheelEvent(QWheelEvent * event);
	
signals:
	
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
