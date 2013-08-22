//
//  BlockViewMap.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/2/13.
//
//

#include "BlockViewMap.h"
#include <QPainter>
#include <QWheelEvent>

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

void BlockViewMap::mouseMoveEvent(QMouseEvent *event)
{
	if ( alignment == 0 )
	{
		return;
	}
	
	BlockView::mouseMoveEvent(event);
	
	if ( mouseDown )
	{
		panToCursor();
	}
}

void BlockViewMap::mousePressEvent(QMouseEvent * event)
{
	BlockView::mousePressEvent(event);
	
	mouseDown = true;
	panToCursor();
}

void BlockViewMap::mouseReleaseEvent(QMouseEvent * event)
{
	BlockView::mouseReleaseEvent(event);
	
	mouseDown = false;
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

void BlockViewMap::updateBuffer()
{
	BlockView::updateBuffer();
	
	if ( alignment == 0 )
	{
		return;
	}
	
	drawSnps();
}

void BlockViewMap::wheelEvent(QWheelEvent * event)
{
	float zoom = 1;
	float zoomFactor = 1 + qAbs((float)event->delta()) / 400;
	
	if ( event->delta() > 0 )
	{
		zoom /= zoomFactor;
	}
	else
	{
		zoom *= zoomFactor;
	}
	
	int focus = (start + end) / 2;
	int size = end - start + 1;
	
	size *= zoom;
	
	if ( size > refSize )
	{
		size = refSize;
	}
	
	// TODO max zoom
	
	start = focus - size / 2;
	end = focus + size / 2;
	
	if ( start < 0 )
	{
		start = 0;
		end = size - 1;
	}
	
	if ( end >= refSize )
	{
		end = refSize;
		start = end - size + 1;
	}
	
	emit signalWindowChanged(start, end);
	setUpdateNeeded();
}

void BlockViewMap::panToCursor()
{
	int focus = (float)getCursorX() / getWidth() * refSize;
	int size = end - start + 1;
	
	start = focus - size / 2;
	
	if ( start < 0 )
	{
		start = 0;
	}
	
	end = start + size - 1;
	
	if ( end >= refSize )
	{
		end = refSize - 1;
		start = end - size + 1;
	}
	
	emit signalWindowChanged(start, end);
}
