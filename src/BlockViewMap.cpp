//
//  BlockViewMap.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/2/13.
//
//

#include "BlockViewMap.h"
#include <QPainter>

void BlockViewMap::setTrackZoom(int newTop, int newBottom)
{
	top = newTop;
	bottom = newBottom;
	update();
}

void BlockViewMap::setWindow(int newStart, int newEnd)
{
	start = newStart;
	end = newEnd;
	update();
}

void BlockViewMap::paintEvent(QPaintEvent *event)
{
	BlockView::paintEvent(event);
	
	QPainter painter(this);
	
	painter.setClipRect(frameWidth(), frameWidth(), width() - frameWidth() * 2, height() - frameWidth() * 2);
	
	QPen pen;
	pen.setColor(qRgb(0, 255, 255));
	pen.setWidth(2);
	pen.setCapStyle(Qt::SquareCap);
	
	painter.setPen(pen);
	int left = (float)start * getWidth() / refSize + 1;
	int width = (float)(end - start + 1) * getWidth() / refSize - 2;
	painter.drawRect(left + frameWidth(), top + frameWidth() + 1, width, bottom - top - 2);
}
