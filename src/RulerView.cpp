// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "RulerView.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <stdio.h>

RulerView::RulerView(QWidget *parent)
: DrawingArea(parent)
{
	alignment = 0;
	position = -1;
	cursorX = -1;
	cursorXDrag = -1;
	setMouseTracking(true);
	//setLineWidth(0);
	updateNeeded = false;
}

void RulerView::clear()
{
	alignment = 0;
	setBufferUpdateNeeded();
}

void RulerView::setAlignment(const Alignment *newAlignment)
{
	alignment = newAlignment;
	setWindow(0, alignment->getLength() - 1);
	updateNeeded = false;
}

void RulerView::setPosition(int gapped, int, int)
{
	position = gapped;
	updateNeeded = true;
//	setBufferUpdateNeeded();
}

void RulerView::setWindow(int newStart, int newEnd)
{
	start = newStart;
	end = newEnd;
	Alignment::Position startPos = alignment->getPositionUngapped(start);
	Alignment::Position endPos = alignment->getPositionUngapped(end);
	startAbs = startPos.abs;
	endAbs = endPos.abs;
	
	if ( startPos.gap )
	{
		startAbs++;
	}
	
	updateNeeded = true;
	updatePosition();
	setBufferUpdateNeeded();
}

void RulerView::update()
{
	if ( updateNeeded )
	{
		DrawingArea::update();
		updateNeeded = false;
	}
}

void RulerView::leaveEvent(QEvent * event)
{
	DrawingArea::leaveEvent(event);
	cursorX = -1;
	emit positionChanged(-1);
}

void RulerView::mouseMoveEvent(QMouseEvent * event)
{
	DrawingArea::mouseMoveEvent(event);
	
	if ( ! alignment )
	{
		return;
	}
	
	int x = event->pos().x() - frameWidth();
//	int y = event->pos().y() - frameWidth();
	
	if ( x >= 0 && x < getWidth() )// && y >= 0 && y < getHeight() )
	{
		cursorX = x;
		updatePosition();
	}
	else
	{
		cursorX = -1;
	}
}

void RulerView::mousePressEvent(QMouseEvent * event)
{
	DrawingArea::mousePressEvent(event);
	
	if ( ! alignment )
	{
		return;
	}
	
	positionDrag = position;
	cursorXDrag = cursorX;
}

void RulerView::mouseReleaseEvent(QMouseEvent *event)
{
	DrawingArea::mouseReleaseEvent(event);
	
	if ( ! alignment )
	{
		return;
	}
	
	if ( cursorXDrag != -1 && cursorX != -1 )
	{
		if ( cursorX == cursorXDrag )
		{
			int windowSize = (end - start + 1);
			emit signalWindowTarget(positionDrag - windowSize / 2, positionDrag + windowSize / 2);
		}
		else
		{
			emit signalWindowTarget(position < positionDrag ? position : positionDrag, position > positionDrag ? position : positionDrag);
		}
	}
	
	cursorXDrag = -1;
}

void RulerView::paintEvent(QPaintEvent *event)
{
	DrawingArea::paintEvent(event);
	
	if ( cursorXDrag != -1 )
	{
		if ( cursorX != -1 )
		{
			QPainter painter(this);
			
			painter.fillRect(cursorXDrag + frameWidth(), frameWidth(), cursorX - cursorXDrag + 1, getHeight(), QColor::fromRgba(qRgba(0, 255, 255, 64)));
		}
	}
	else if ( position != -1 )
	{
		int x1 = int((position - start) * (float)getWidth() / (end - start + 1)) + frameWidth();
		int x2 = int((position - start + 1) * (float)getWidth() / (end - start + 1)) + frameWidth() - 1;
		
		QPainter painter(this);
		
		painter.setPen(qRgb(0, 255, 255));
		painter.drawLine(x1 - 1, 0, x1 - 1, height());
		painter.drawLine(x1 + 1, 0, x1 + 1, height());
		
		if ( x2 > x1 )
		{
			painter.drawLine(x2 - 1, 0, x2 - 1, height());
			painter.drawLine(x2 + 1, 0, x2 + 1, height());
		}
		
		painter.setPen(qRgb(0, 155, 155));
		painter.drawLine(x1, 0, x1, height());
		
		if ( x2 > x1 )
		{
			painter.drawLine(x2, 0, x2, height());
		}
	}
}

void RulerView::wheelEvent(QWheelEvent * event)
{
	if ( alignment )
	{
		emit signalMouseWheel(event->delta());
	}
}

void RulerView::updateBuffer()
{
	if ( alignment == 0 || imageBuffer == 0 )
	{
		clearBuffer();
		return;
	}
	
	if ( width() <= frameWidth() * 2 )
	{
		return;
	}
	
	float fontSize = height() * .8;
	
	if ( fontSize > 12 )
	{
		fontSize = 12;
	}
	
	QPainter painter(imageBuffer);
	QFont font;
	font.setPixelSize(fontSize);
	painter.setFont(font);//fontSize));
	
	//clearBuffer();
	imageBuffer->fill(qRgb(255, 255, 255));
	
	int lengthGapped = end - start + 1;
	int lengthAbs = endAbs - startAbs + 1;
	
	for ( int i = 0; i < getWidth(); i++ )
	{
		Alignment::Position pos = alignment->getPositionUngapped(start + i * float(end - start) / getWidth() );
		
		int shade = 255;
		
		for ( int j = 1; j < lengthAbs; j *= 10 )
		{
			if ( ! (pos.abs / j & 1) )
			{
				shade -= 10. / qPow(qLn((float)j * 10. / lengthAbs), 2);
			}
		}
		
		//((QRgb *)image.scanLine(0))[i] = qRgb(shade, shade, shade);
	}
	
	int min = qPow(10, int(qLn(lengthAbs) / qLn(10))) / 1000;
	
	if ( min == 0 )
	{
		min = 1;
	}
	
	int darkness = 1;
	int minLabel = 1;
	int factor = 2;
	
	for ( int i = min; i < lengthAbs * 10; i *= factor )
	{
		if ( factor == 5 )
		{
			factor = 2;
		}
		else
		{
			factor = 5;
		}
		
		int shade = 250 - getWidth() * (float)i / lengthGapped / 1.2;
		
		if ( shade > 245 )
		{
			continue;
		}
		
		//int shade = 255 - darkness;
		if ( shade > 255 )
		{
			shade = 255;
		}
		if ( shade < 0 )
		{
			shade = 0;
		}
		darkness *= 8;
		
		int delta = i / 2;
		
		if ( delta == 0 )
		{
			delta = 1;
		}
		
		for ( int j = startAbs / i * i - 1; j <= endAbs + 1; j += i )
		{
			int gapped = alignment->getPositionGapped(j);
			
			if ( gapped < start || j == 0 )
			{
				continue;
			}
			
			int bin = int((gapped - start) * (float)getWidth() / (end - start + 1));
			
			if ( bin < 0 )
			{
				continue;
			}
			
			if ( shade > 0 )
			{
				painter.setPen(qRgb(shade, shade, shade));
				painter.drawLine(bin, 0, bin, getHeight());
			}
		}
		
		if ( minLabel == 1 && factor == 5 && getWidth() * (float)i / lengthGapped > 30 )
		{
			minLabel = i;
		}
//		max = i;
	}
	
	for ( int i = 1; i < getHeight(); i++ )
	{
		//memcpy(imageBuffer->scanLine(i), imageBuffer->scanLine(0), sizeof(char) * 4 * imageBuffer->width());
	}
	
	int digits = (float)(qLn(minLabel) / qLn(10));
	int unit = digits / 3;
	factor = qPow(10, 3 * unit);
	
	char units[4] = {' ', 'K', 'M', 'G'};
	
	for ( int j = startAbs / minLabel * minLabel; j <= endAbs + 1; j += minLabel )
	{
		int gapped = alignment->getPositionGapped(j);
		
		if ( gapped < start || j == 0 )
		{
			continue;
		}
		
		int bin = int((gapped - start - 1) * (float)getWidth() / lengthGapped);
		
		if ( bin < 0 )
		{
			continue;
		}
		
		int left = j / (factor * 1000);
		int right = j % (factor * 1000) / factor;
		char rightString[16];
		char rightStringFormat[16];
		int shadeLeft = 250 - (getWidth() * (float)minLabel / lengthGapped - 60) * 3.;
		int shadeRight = 250 - (getWidth() * (float)minLabel / lengthGapped - 30) * 5;
		int shadeRightRed = 250 - (getWidth() * (float)minLabel / lengthGapped - 30) * 2;
		int minRed = 150;
		
		if ( shadeLeft < 0 )
		{
			shadeLeft = 0;
		}
		
		if ( shadeRight < 0 )
		{
			shadeRight = 0;
		}
		
		if ( shadeRightRed < minRed )
		{
			shadeRightRed = minRed;
		}
		
		if ( j % (minLabel * 10) == 0 || getWidth() * (float)minLabel / lengthGapped > 60 )
		{
			if ( left )
			{
				if ( j % (minLabel * 10) == 0 )
				{
					painter.setPen(qRgb(0, 0, 0));
					shadeRight = 0;
					shadeRightRed = minRed;
				}
				else
				{
//					int shade = 250 - getWidth() * (float)minLabel / lengthGapped / 1.2;
					painter.setPen(qRgb(shadeLeft, shadeLeft, shadeLeft));
				}
				
				painter.drawText(QRect(0, 0, bin, getHeight()), Qt::AlignRight | Qt::AlignVCenter, QString::number(left));
			}
			
			painter.setPen(qRgb(200, shadeLeft, shadeLeft));
		}
		else
		{
			painter.setPen(qRgb(200, shadeRight, shadeRight));
		}
		
		painter.setPen(qRgb(shadeRightRed, shadeRight, shadeRight));
		
		if ( left )
		{
			sprintf(rightStringFormat, "%%0%dd%%c", 3);
			sprintf(rightString, rightStringFormat, right, units[unit]);
		}
		else
		{
			sprintf(rightString, "%d%c", right, units[unit]);
		}
		
		painter.drawText(QRect(bin + 2, 0, getWidth(), getHeight()), Qt::AlignVCenter, QString(rightString));
	}
}

void RulerView::updatePosition()
{
	if ( cursorX != -1 )
	{
		unsigned int focus = start + float(end - start + 1) * (float(cursorX) / getWidth());
		
		emit positionChanged(focus);
	}
}
