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
	imageBuffer = new QImage(getWidth(), getHeight(), QImage::Format_RGB32);
	
	setAutoFillBackground(false);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);
	//viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
	//viewport()->setAttribute(Qt::WA_NoSystemBackground);
}

DrawingArea::~DrawingArea()
{
	if ( imageBuffer )
	{
		delete imageBuffer;
	}
}

void DrawingArea::clearBuffer() const
{
	imageBuffer->fill(qRgb(245, 245, 245));
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
	QFrame::paintEvent(event);
    
    if ( bufferUpdateNeeded )
    {
		if ( imageBuffer && (getWidth() != imageBuffer->width() || getHeight() != imageBuffer->height()) )
		{
			if ( imageBuffer )
			{
				delete imageBuffer;
			}
			
			imageBuffer = new QImage(getWidth(), getHeight(), QImage::Format_RGB32);
		}
		
        updateBuffer();
        bufferUpdateNeeded = false;
    }
    
	QPainter painter(this);
	painter.drawImage(frameWidth() + xOffset, frameWidth(), *imageBuffer);
}

void DrawingArea::resizeEvent(QResizeEvent * event)
{
	
	if ( imageBuffer )
	{
//		delete imageBuffer;
	}
	int width = getWidth();
	int height = getHeight();
	
//	imageBuffer = new QImage(getWidth(), getHeight(), QImage::Format_RGB32);
	QFrame::resizeEvent(event);
	
	int width2 = getWidth();
	int height2 = getHeight();
	setBufferUpdateNeeded();
}

void DrawingArea::updateBuffer()
{
	
}
