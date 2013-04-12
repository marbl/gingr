//
//  DrawingArea.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "DrawingArea.h"
#include <QPainter>

DrawingArea::DrawingArea(QWidget *parent)
: QFrame(parent)
{
	setFrameStyle(QFrame::Panel | QFrame::Sunken);
	setLineWidth(1);
//	setStyleSheet("* { background-color: rgb(245, 245, 245); }");
	xOffset = 0;
	imageBuffer = 0;
}

DrawingArea::~DrawingArea()
{
	if ( imageBuffer )
	{
		delete imageBuffer;
	}
}

void DrawingArea::clearBuffer()
{
	imageBuffer->fill(qRgb(245, 245, 245));
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
	QFrame::paintEvent(event);
    
    if ( bufferUpdateNeeded )
    {
        updateBuffer();
        bufferUpdateNeeded = false;
    }
    
	QPainter painter(this);
	painter.drawImage(frameWidth() + xOffset, frameWidth(), *imageBuffer);
}

void DrawingArea::resizeEvent(QResizeEvent * event)
{
	QFrame::resizeEvent(event);
	
	if ( imageBuffer )
	{
		delete imageBuffer;
	}
	
	imageBuffer = new QImage(getWidth(), getHeight(), QImage::Format_RGB32);
	setBufferUpdateNeeded();
}

void DrawingArea::updateBuffer()
{
	
}
