//
//  SnpLegend.cpp
//  gavqt
//
//  Created by Brian Ondov on 6/17/13.
//
//

#include "SnpLegend.h"
#include <QImage>
#include <QPainter>

SnpLegend::SnpLegend()
{
	baseBufferRef = 0;
	baseBufferSnp = 0;
	show = false;
	showBases = false;
	showSynteny = false;
}

SnpLegend::~SnpLegend()
{
	if ( baseBufferRef )
	{
		delete baseBufferRef;
	}
	
	if ( baseBufferSnp )
	{
		delete baseBufferSnp;
	}
}

void SnpLegend::setShow(bool newShow)
{
	show = newShow;
	setBufferUpdateNeeded();
}

void SnpLegend::setShowBases(bool newShowBases)
{
	if ( newShowBases != showBases )
	{
		setBufferUpdateNeeded();
	}
	
	showBases = newShowBases;
}

void SnpLegend::setShowSynteny(bool newShowSynteny)
{
	if ( newShowSynteny != showSynteny )
	{
		setBufferUpdateNeeded();
	}
	
	showSynteny = newShowSynteny;
}

void SnpLegend::resizeEvent(QResizeEvent * event)
{
	DrawingArea::resizeEvent(event);
	
	if ( baseBufferRef )
	{
		delete baseBufferRef;
	}
	
	if ( baseBufferSnp )
	{
		delete baseBufferSnp;
	}
	
	baseSize = getWidth() / (baseCount * 2);
	
	baseBufferRef = new BaseBuffer(baseSize, getHeight(), false, true);
	baseBufferSnp = new BaseBuffer(baseSize, getHeight(), true);
}

void SnpLegend::updateBuffer()
{
	if ( ! show )
	{
		clearBuffer();
		return;
	}
	
	if ( showBases )
	{
		QPainter painter(imageBuffer);
		
		for ( int i = 0; i < baseCount * 2; i++ )
		{
			const QPixmap * pixmap;
			
			if ( i < baseCount )
			{
				pixmap = baseBufferRef->image(bases[i]);
			}
			else
			{
				pixmap = baseBufferSnp->image(bases[i - baseCount]);
			}
			
			painter.drawPixmap(i * baseSize, 0, *pixmap);
		}
	}
	else
	{
		int paletteSize = showSynteny ? SyntenyPalette::PALETTE_SIZE : SnpPalette::PALETTE_SIZE;
		
		for ( int i = 0; i < getWidth(); i++ )
		{
			int index;
			
			if ( i == getWidth() - 1 )
			{
				index = paletteSize - 1;
			}
			else
			{
				index = (float)i * paletteSize / getWidth();
			}
			
			((QRgb *)imageBuffer->scanLine(0))[i] = showSynteny ? paletteSynteny.color(index) : palette.color(index);
		}
		
		for ( int i = 1; i < imageBuffer->height(); i++ )
		{
			memcpy(imageBuffer->scanLine(i), imageBuffer->scanLine(0), sizeof(QRgb) * getWidth());
		}
	}
}
