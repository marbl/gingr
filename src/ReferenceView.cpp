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

ReferenceView::ReferenceView()
{
	snpBuffer = 0;
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
	posStart = newPosStart;
	posEnd = newPosEnd;
	setBufferUpdateNeeded();
}

void ReferenceView::updateSnpsFinished()
{
	setBufferUpdateNeeded();
}

void ReferenceView::updateBuffer()
{
	clearBuffer();
	
	if ( ! snpBuffer )
	{
		return;
	}
	
	if ( ! snpBuffer->ready() || snpBuffer->getPosStart() > posStart || snpBuffer->getPosEnd() < posEnd )
	{
		printf("Clearing...\n");
		imageBuffer->fill(qRgb(0, 0, 0));
		//clearBuffer();
	}
	
	snpBuffer->drawSnpSums(imageBuffer, 0, getHeight() - 1, posStart, posEnd, getWidth());
	
	float baseWidth = (float)getWidth() / (posEnd - posStart);
	
	QPainter painter(imageBuffer);
	
	if ( baseWidth < 1 )
	{
		return;
	}
	
	const BaseBuffer * baseBufferRef = new BaseBuffer(baseWidth, getHeight() - 1, false);
	
	QImage imageRef(getWidth(), getHeight(), QImage::Format_RGB32);
	QPainter painterRef(&imageRef);
	
	//imageRef.fill(qRgb(80, 80, 80));
	
	for ( int i = 0; i < posEnd - posStart + 1; i++ )
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
		int x = i * getWidth() / (posEnd - posStart + 1);
		
		const QPixmap * charImage = baseBufferRef->image(alignment->getRefSeqGapped()[i + posStart]);
		
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

