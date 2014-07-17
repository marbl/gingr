
//
//  BlockView.cpp
//  gavqt
//
//  Created by Brian Ondov on 11/6/12.
//
//

#include "BlockView.h"
#include <QPainter>
#include <string.h>
#include <QtCore/qmath.h>

BlockView::BlockView()
: TrackListView()
{
	alignment = 0;
	snpsCenter = 0;
	lightColors = false;
}

BlockView::~BlockView()
{
}

void BlockView::clear()
{
	setTrackFocus(-1);
	alignment = 0;
	setBufferUpdateNeeded();
}

void BlockView::update()
{
	if ( false && snpMaxTimer.update() )
	{
		setUpdateNeeded();
		snpMax.update(snpMaxTimer.getProgress());
		
		if ( snpMaxCur != snpMaxTarget )
		{
			
			int delta = .5 * (snpMaxTarget - snpMaxCur);
			
			if ( abs(delta) < .05 * snpMaxTarget )
			{
				snpMaxCur = snpMaxTarget;
			}
			else
			{
				if (false && abs(delta) > .3 * snpMaxCur )
				{
					delta = .3 * snpMaxCur * (snpMaxCur > snpMaxTarget ? -1 : 1);
				}
				
				if ( delta == 0 )
				{
					delta = snpMaxTarget > snpMaxCur ? 1 : -1;
				}
				
				snpMaxCur += delta;
			}
		}
	}
	
	snpMaxCur = snpMaxTarget;
	
	TrackListView::update();
}

void BlockView::setAlignment(const Alignment *newAlignment)
{
	alignment = newAlignment;
	
	refSize = alignment->getLength();
	
	posStart = 0;
	posEnd = refSize - 1;
	
	snpMax.initialize(0, 0);
	snpMaxCur = 0;
	snpMaxTarget = 0;
	
	emit windowChanged(posStart, posEnd);
	//updateSnps();
}

void BlockView::setSnpBuffer(const SnpBuffer *newSnpBuffer)
{
	snpsCenter = newSnpBuffer;
}

void BlockView::updateSnpsFinished()
{
	//unsigned int snpMaxTarget = 0;
	snpMaxTarget = 0;
	/*
	for ( int i = 0; i < getTrackCount(); i++ )
	{
		for ( unsigned int j = 0; j < snpsCenter->getBins(); j++ )
		{
			if ( snpsCenter->getSnps()[i][j] > snpMaxTarget )
			{
				snpMaxTarget = snpsCenter->getSnps()[i][j];
			}
		}
	}
	*/
	//snpMaxTarget = snpsCenter->getSnpMax();
	snpMax.setTarget(snpMaxTarget, (snpMaxTarget - snpMax.getEnd()) / snpMax.getEnd() < .2);
	snpMaxTimer.initialize(320);
	/*
	if ( false && timeZoomIn.msecsTo(QDateTime::currentDateTime()) > 50 )
	{
		printf("updating left: %d\t%d\n", posStart * 2 - posEnd - 1, posStart - 1);
		snpsLeft.update(posStart * 2 - posEnd - 1, posStart - 1, width() - frameWidth() * 2);
		printf("updating right: %d\t%d\n", posEnd + 1, 2 * posEnd - posStart + 1);
		snpsRight.update(posEnd + 1, 2 * posEnd - posStart + 1, width() - frameWidth() * 2);
	}
	*/
	lightColors = snpsCenter->getLightColors();
	setBufferUpdateNeeded();
	setUpdateNeeded();
}

void BlockView::updateSnpsNeeded()
{
	updateSnps();
}

inline float absf(float f) {return f > 0 ? f : -f;}

void BlockView::updateBuffer()
{
	if ( alignment == 0 )
	{
		clearBuffer();
	}
}

void BlockView::paintEvent(QPaintEvent * event)
{
	DrawingArea::paintEvent(event);
	
	if ( alignment == 0 || imageBuffer == 0 )
	{
		return;
	}
	
	if ( width() <= frameWidth() * 2 )
	{
		return;
	}
	
	QPainter painter(this);
//	imageBuffer->fill(qRgb(255, 255, 255));
	int lcbHoverX = 0;
//	unsigned int totals[getTrackCount()][image.width()];
	
	if ( getTrackHover() != -1 )
	{
		if ( lightColors )
		{
			painter.setPen(qRgb(0, 0, 0));
		}
		else
		{
			painter.setPen(qRgb(255, 255, 255));
		}
		
		painter.drawLine(frameWidth() + xOffset, (int)getTrackHeight(getTrackHover()) + frameWidth() + .5, width() - frameWidth() - 1 + xOffset, (int)getTrackHeight(getTrackHover()) + frameWidth() + .5);
		painter.drawLine(frameWidth() + xOffset, (int)getTrackHeight(getTrackHoverEnd() + 1) + frameWidth() + .5, width() - frameWidth() - 1 + xOffset, (int)getTrackHeight(getTrackHoverEnd() + 1) + frameWidth() + .5);
	}
}

void BlockView::resizeEvent(QResizeEvent * event)
{
	TrackListView::resizeEvent(event);
	
	if ( alignment )
	{
		updateSnps();
	}
}

void BlockView::drawSnps() const
{
	if ( ! snpsCenter || ! snpsCenter->ready() || snpsCenter->getPosStart() > posStart || snpsCenter->getPosEnd() < posEnd )
	{
		//printf("Clearing...\n");
		if ( lightColors )
		{
			imageBuffer->fill(qRgb(255, 255, 255));
		}
		else
		{
			imageBuffer->fill(qRgb(0, 0, 0));
		}
		//clearBuffer();
	}
	
	if ( snpsCenter == 0 || ! snpsCenter->ready() )
	{
		return;
	}
	
	for ( int i = getTrackCount() - 1; i >= 0; i-- )
	{
		if ( getTrackHeight(i + 1) < 0 || getTrackHeight(i) >= getHeight() )
		{
			continue;
		}
		
		int bottom = getTrackHeight(i + 1) - 1;
		
		if ( bottom >= getHeight() )
		{
			bottom = getHeight() - 1;
		}
		
		snpsCenter->drawSnps(imageBuffer, getIdByTrack(i), getTrackHeight(i), bottom, posStart, posEnd, getWidth());
		//snpsCenter->drawSnps(imageBuffer, getIdByTrack(i), getTrackHeight(i), bottom, posStart, posEnd, getWidth()); // TEMP
	}
}

void BlockView::updateSnps()
{
	if ( width() <= frameWidth() * 2 )
	{
		return;
	}
	
	emit signalUpdateSnps();
//	printf("updating cent: %d\t%d\n", posStart, posEnd);
//	snpsCenter->update(posStart, posEnd, width() - frameWidth() * 2);
//	snpsCenter->update(posStart * 2 - posEnd - 1, 2 * posEnd - posStart + 1, 3 * (width() - frameWidth() * 2));
	/*
	if ( false && timeZoomOut.msecsTo(QDateTime::currentDateTime()) < 50 )
	{
		printf("updating left: %d\t%d\n", posStart * 2 - posEnd - 1, posStart - 1);
		snpsLeft.update(posStart * 2 - posEnd - 1, posStart - 1, width() - frameWidth() * 2);
		printf("updating right: %d\t%d\n", posEnd + 1, 2 * posEnd - posStart + 1);
		snpsRight.update(posEnd + 1, 2 * posEnd - posStart + 1, width() - frameWidth() * 2);
	}*/
}
