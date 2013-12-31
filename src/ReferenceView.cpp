//
//  ReferenceView.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/9/13.
//
//

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
	
	if ( ! snpBuffer )
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
	
	if ( baseWidth < 1 )
	{
		return;
	}
	
	bool showGaps = snpBuffer->getShowGaps() & Alignment::SHOW && snpBuffer->getShowGaps() & Alignment::INSERTIONS;
	const BaseBuffer * baseBufferRef = new BaseBuffer(baseWidth, getHeight() - 1, lightColors, false, showGaps);
	
	QImage imageRef(getWidth(), getHeight(), QImage::Format_RGB32);
	QPainter painterRef(&imageRef);
	
	//imageRef.fill(qRgb(80, 80, 80));
	
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
		
		const QPixmap * charImage = baseBufferRef->image(alignment->getRefSeqGapped()[i + start]);
		
		if ( charImage )
		{
			painterRef.drawPixmap(x, 0, *charImage);
		}
	}
	
	if ( baseWidth < 2 )
	{
		painter.setOpacity(baseWidth - 1);
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
