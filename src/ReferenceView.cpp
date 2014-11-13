// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "ReferenceView.h"
#include <QPainter>
#include "BaseBuffer.h"
#include <QMouseEvent>

ReferenceView::ReferenceView()
{
	snpBuffer = 0;
	lightColors = false;
	cursorX = -1;
	setMouseTracking(true);
}

void ReferenceView::clear()
{
	alignment = 0;
	setBufferUpdateNeeded();
}

void ReferenceView::setAlignment(const Alignment *newAlignment)
{
	alignment = newAlignment;
}

void ReferenceView::setSnpBuffer(const SnpBuffer * newSnpBuffer)
{
	snpBuffer = newSnpBuffer;
}

void ReferenceView::setWindow(int newPosStart, int newPosEnd)
{
	start = newPosStart;
	end = newPosEnd;
	updatePosition();
	setBufferUpdateNeeded();
}

void ReferenceView::updateSnpsFinished()
{
	lightColors = snpBuffer->getLightColors();
	setBufferUpdateNeeded();
}

void ReferenceView::leaveEvent(QEvent * event)
{
	DrawingArea::leaveEvent(event);
	cursorX = -1;
	emit positionChanged(-1);
}

void ReferenceView::mouseMoveEvent(QMouseEvent * event)
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

void ReferenceView::updateBuffer()
{
	clearBuffer();
	
	if ( ! alignment || ! snpBuffer || ! snpBuffer->ready() )
	{
		return;
	}
	
	if ( ! snpBuffer->ready() || snpBuffer->getPosStart() > start || snpBuffer->getPosEnd() < end )
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
	
	snpBuffer->drawSnpSums(imageBuffer, 0, getHeight() - 1, start, end, getWidth());
	
	float baseWidth = (float)getWidth() / (end - start);
	
	QPainter painter(imageBuffer);
	
	if ( snpBuffer->getMax() != 1 && baseWidth < 1 || snpBuffer->getSynteny() )
	{
		return;
	}
	
	bool showGaps = snpBuffer->ready() && snpBuffer->getShowGaps() & Alignment::SHOW && snpBuffer->getShowGaps() & Alignment::INSERTIONS;
	BaseBuffer baseBufferRef(baseWidth, getHeight() - 1, lightColors, false, showGaps);
	const BaseBuffer * baseBufferSnp = 0;
	
	QImage imageRef(getWidth(), getHeight(), QImage::Format_RGB32);
	QPainter painterRef(&imageRef);
	
	//imageRef.fill(qRgb(80, 80, 80));
	
	
	if ( baseWidth > 1 )
	{
		for ( int i = 0; i < end - start + 1; i++ )
		{
			/*
			int bin =
			(float)i /
			float(snpBuffer->getPosEnd() - snpBuffer->getPosStart()) *
			snpBuffer->getBins() +
			float(posStart - snpBuffer->getPosStart()) *
			snpBuffer->getBins() /
			(snpBuffer->getPosEnd() - snpBuffer->getPosStart());
			
			if ( snpBuffer->getLcbs()[bin] == 0 )
			{
				continue;
			}
			*/
			int x = i * getWidth() / (end - start + 1);
			
			const QPixmap * charImage = baseBufferRef.image(alignment->getRefSeqGapped()[i + start]);
			
			if ( charImage )
			{
				painterRef.drawPixmap(x, 0, *charImage);
			}
		}
	}
	else
	{
		imageRef.fill(qRgb(255, 255, 255));
	}
	
	unsigned int firstSnp = alignment->getNextSnpIndex(start);
	
	//if ( alignment->getTrackReference() != 0 )
	{
		for ( int i = firstSnp; i < alignment->getSnpColumnCount() && alignment->getSnpColumn(i).position >= start && alignment->getSnpColumn(i).position <= end; i++ )
		{
			const Alignment::SnpColumn & snpColumn = alignment->getSnpColumn(i);
			char ref = alignment->getRefSeqGapped()[snpColumn.position];
			
			if ( snpColumn.ref != ref )
			{
				int x = float(snpColumn.position - start) * getWidth() / (end - start + 1);
				const QPixmap * charImage = 0;
				
				if ( alignment->filter(snpColumn.filters) )
				{
					if ( baseBufferSnp == 0 )
					{
						baseBufferSnp = new BaseBuffer(baseWidth, getHeight() - 1, lightColors, true, showGaps);
					}
					
					charImage = baseBufferSnp->image(ref);
				}
				else if ( baseWidth > 1 )
				{
					charImage = baseBufferRef.image(ref);
				}
				
				if ( charImage )
				{
					painterRef.drawPixmap(x, 0, *charImage);
				}
			}
		}
	}
	
	if ( baseBufferSnp )
	{
		delete baseBufferSnp;
	}
	
	if ( baseWidth < 2 )
	{
		//painter.setOpacity(baseWidth - 1);
	}
	
	painter.drawImage(0, 0, imageRef);
}

void ReferenceView::wheelEvent(QWheelEvent * event)
{
	DrawingArea::wheelEvent(event);
	emit signalMouseWheel(event->delta());
}

void ReferenceView::updatePosition()
{
	if ( cursorX != -1 )
	{
		unsigned int focus = start + float(end - start + 1) * (float(cursorX) / getWidth());
		
		emit positionChanged(focus);
	}
}
