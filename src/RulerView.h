// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__RulerView__
#define __gavqt__RulerView__

#include "DrawingArea.h"
#include "Alignment.h"

class RulerView : public DrawingArea
{
	Q_OBJECT
	
public:
	
	RulerView(QWidget *parent = 0);
	
	void clear();
	void setAlignment(const Alignment * newAlignment);
	void setPosition(int gapped, int ungapped, int offset);
	void setWindow(int newStart, int newEnd);
	void update();
	
signals:
	
	void positionChanged(int ungapped);
	void signalMouseWheel(int delta);
	void signalWindowTarget(int start, int end);
	
protected:
	
	void leaveEvent(QEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event);
	void updateBuffer();
	void wheelEvent(QWheelEvent * event);
	
private:
	
	void updatePosition();
	
	const Alignment * alignment;
	int start;
	int end;
	int startAbs;
	int endAbs;
	bool updateNeeded;
	int position;
	int cursorX;
	int cursorXDrag;
	int positionDrag;
};

#endif /* defined(__gavqt__RulerView__) */
