//
//  RulerView.h
//  gavqt
//
//  Created by Brian Ondov on 2/7/13.
//
//

#ifndef __gavqt__RulerView__
#define __gavqt__RulerView__

#include "DrawingArea.h"
#include "Alignment.h"

class RulerView : public DrawingArea
{
	Q_OBJECT
	
public:
	
	RulerView(QWidget *parent = 0);
	void setAlignment(const Alignment * newAlignment);
	void setPosition(int gapped, int ungapped, int offset);
	void setWindow(unsigned int newStart, unsigned int newEnd);
	void update();
	
protected:
	
	void paintEvent(QPaintEvent * event);
	
private:
	
	const Alignment * alignment;
	int start;
	int end;
	int startAbs;
	int endAbs;
	bool updateNeeded;
	int position;
};

#endif /* defined(__gavqt__RulerView__) */
