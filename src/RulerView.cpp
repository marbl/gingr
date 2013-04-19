//
//  RulerView.cpp
//  gavqt
//
//  Created by Brian Ondov on 2/7/13.
//
//

#include "RulerView.h"
#include <QPainter>
#include <QtCore/qmath.h>

RulerView::RulerView(QWidget *parent)
: DrawingArea(parent)
{
	alignment = 0;
	//setLineWidth(0);
}

void RulerView::setAlignment(const Alignment *newAlignment)
{
	alignment = newAlignment;
	setWindow(0, alignment->getLength() - 1);
	updateNeeded = false;
}

void RulerView::setPosition(int gapped, int ungapped, int offset)
{
	position = gapped;
	updateNeeded = true;
}

void RulerView::setWindow(unsigned int newStart, unsigned int newEnd)
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
}

void RulerView::update()
{
	if ( updateNeeded )
	{
		DrawingArea::update();
		updateNeeded = false;
	}
}

void RulerView::paintEvent(QPaintEvent * event)
{
	DrawingArea::paintEvent(event);
	
	if ( alignment == 0 )
	{
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
	
	QPainter painter(this);
	QFont font;
	font.setPixelSize(fontSize);
	painter.setFont(font);//fontSize));
	QImage image(width() - frameWidth() * 2, height() - frameWidth() * 2, QImage::Format_RGB32);
	
	image.fill(qRgb(245, 245, 245));
	
	int lengthAbs = endAbs - startAbs + 1;
	
	for ( int i = 0; i < image.width(); i++ )
	{
		Alignment::Position pos = alignment->getPositionUngapped(start + i * float(end - start) / image.width() );
		
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
	int max;
	int darkness = 1;
	int minLabel = 1;
	
	for ( int i = min; i < lengthAbs; i *= 10 )
	{
		int shade = 250 - image.width() * (float)i / lengthAbs / 1.2;
		
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
		
		for ( int j = startAbs / i * i; j <= endAbs + 1; j += i )
		{
			int gapped = alignment->getPositionGapped(j);
			
			if ( gapped < start || j == 0 )
			{
				continue;
			}
			
			int bin = int((gapped - start - 1) * (float)image.width() / (end - start + 1));
			
			if ( bin < 0 )
			{
				continue;
			}
			
			((QRgb *)image.scanLine(0))[bin] = qRgb(shade, shade, shade);
		}
		
		if ( minLabel == 1 && image.width() * (float)i / lengthAbs > 30 )
		{
			minLabel = i;
		}
//		max = i;
	}
	
	for ( int i = 1; i < image.height(); i++ )
	{
		memcpy(image.scanLine(i), image.scanLine(0), sizeof(char) * 4 * image.width());
	}
	
	int digits = (float)(qLn(minLabel) / qLn(10));
	int unit = digits / 3;
	int factor = qPow(10, 3 * unit);
	
	char units[4] = {' ', 'K', 'M', 'G'};
	painter.drawImage(frameWidth(), frameWidth(), image);
	
	for ( int j = startAbs / minLabel * minLabel; j <= endAbs + 1; j += minLabel )
	{
		int gapped = alignment->getPositionGapped(j);
		
		if ( gapped < start || j == 0 )
		{
			continue;
		}
		
		int bin = int((gapped - start - 1) * (float)image.width() / (end - start + 1));
		
		if ( bin < 0 )
		{
			continue;
		}
		
		int right = j % (factor * 1000) / factor;
		char rightString[16];
		char rightStringFormat[16];
		sprintf(rightStringFormat, "%%0%dd%%c", 3);//digits - unit * 3 + 1);
		sprintf(rightString, rightStringFormat, right, units[unit]);
		int shade = 250 - image.width() * (float)minLabel / lengthAbs / 1.2;
		
		if ( j % (minLabel * 10) == 0 || image.width() * (float)minLabel / lengthAbs > 60 )
		{
			int left = j / (factor * 1000);
			
			if ( j % (minLabel * 10) == 0 )
			{
				painter.setPen(qRgb(0, 0, 0));
			}
			else
			{
				int shade = 250 - image.width() * (float)minLabel / lengthAbs / 1.5;
				painter.setPen(qRgb(shade, shade, shade));
			}
			
			painter.drawText(QRect(0, 0, bin, image.height()), Qt::AlignRight | Qt::AlignVCenter, QString::number(left));
			
			painter.setPen(qRgb(255, 0, 0));
		}
		else
		{
			painter.setPen(qRgb(255, shade, shade));
		}
		
		painter.drawText(QRect(bin + 2, 0, image.width(), image.height()), Qt::AlignVCenter, QString(rightString));
	}
	
	int x = int((position - start - 1) * (float)image.width() / (end - start + 1));
	painter.setPen(qRgb(0, 255, 255));
	painter.drawLine(x - 1, 0, x - 1, height());
	painter.drawLine(x + 1, 0, x + 1, height());
	painter.setPen(qRgb(0, 155, 155));
	painter.drawLine(x, 0, x, height());
}
