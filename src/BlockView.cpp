
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
}

BlockView::~BlockView()
{
}

void BlockView::update()
{
	if ( true || snpMaxTimer.update() )
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
	
	const RegionVector * trackRef = (*alignment->getTracks())[0]; // TODO: ref idx
	refByLcb.resize(alignment->getLcbCount());
	
	unsigned int total = 0; // total collapsed genome size so far
	unsigned int gaps = 0;
	
	for ( int i = 0; i < trackRef->size(); i++ )
	{
		const Region * region = (*trackRef)[i];
		refByLcb[region->getLcb()] = new Region(i, total, region->getLength(), region->getRc(), 0);
		
		total += region->getLength();
		gaps += alignment->getLcb(region->getLcb()).lengthGapped - region->getLength();
		refSize = region->getStart() + region->getLength() + gaps;
	}
	
	//refSize = total;
	
	snpsLeft.initialize(alignment);
	snpsRight.initialize(alignment);
	
	//connect(&snpsLeft, SIGNAL(updated()), this, SLOT(updateSnpsFinished()));
	//connect(&snpsRight, SIGNAL(updated()), this, SLOT(updateSnpsFinished()));
	
	snpsMax = new unsigned int * [getTrackCount()];
	
	for (int i = 0; i < 0*alignment->getTracks()->size(); i++)
	{
		total = 0;
		snpsMax[i] = new unsigned int[SNP_WINDOW];
		memset(snpsMax[i], 0, SNP_WINDOW * sizeof(unsigned int));
		
		for ( int j = 0; j < alignment->getLcbCount(); j++ )
		{
			for ( unsigned int k = 0; k < alignment->getSnpCountByLcb(j); k++ )
			{
				if ( (*(*alignment->getTracks())[getIdByTrack(i)])[j]->getSnp(k) != (*(*alignment->getTracks())[0])[j]->getSnp(k) )
				{
					//int x = float((total + alignment->getSnpPositionsByLcb(j)[k])) / refSize * (SNP_WINDOW - 1);
//		snpsMax[i][x]++;
				}
			}
			
			total += (*(*alignment->getTracks())[0])[j]->getLength();
		}
	}
	
	posStart = 0;
	posEnd = refSize - 1;
	
	snpMax.initialize(0, 0);
	snpMaxCur = 0;
	snpMaxTarget = 0;
	
	emit windowChanged(posStart, posEnd);
	updateSnps();
}

void BlockView::setSnpBuffer(const SnpBuffer *newSnpBuffer)
{
	snpsCenter = newSnpBuffer;
}

void BlockView::updateSnpsFinished()
{
	//unsigned int snpMaxTarget = 0;
	snpMaxTarget = 0;
	
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
	
	snpMaxTarget = snpsCenter->getSnpMax();
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
	setBufferUpdateNeeded();
	setUpdateNeeded();
}

void BlockView::updateSnpsNeeded()
{
	updateSnps();
}

void BlockView::updateBuffer()
{
	drawSnps();
}

inline float absf(float f) {return f > 0 ? f : -f;}

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
	
	for ( int i = getTrackCount() - 1; i >= 0; i-- )
	{
		unsigned int total = 0;
//		memset(totals[i], 0, image.width() * sizeof(unsigned int));
		
//		for ( unsigned int j = 0; j < SNP_WINDOW / zoom; j++ )
//		{
//			totals[i][(int)(j * image.width() * zoom / SNP_WINDOW)] += snpsMax[i][j];
//		}
		
		QRgb * line = (QRgb *)imageBuffer->scanLine(getTrackHeight(i));
		
		int zoom = 0; // TEMP;
		
		for ( int j = 0; j < alignment->getLcbCount(); j++ )
		{break;
			
			for ( int k = total * imageBuffer->width() * zoom / refSize; k <= (total + (*alignment->getLcb(j).regions)[0]->getLength()) * imageBuffer->width() * zoom / refSize; k++ )
			{
				int shade = 255 - qSqrt(absf((*alignment->getLcb(j).regions)[0]->getStartScaled() - (*alignment->getLcb(j).regions)[getIdByTrack(i)]->getStartScaled())) * 256;
				
				if ( k >= imageBuffer->width() )
				{
					continue;
				}
				
				if ( (*alignment->getLcb(j).regions)[0]->getRc() == (*alignment->getLcb(j).regions)[getIdByTrack(i)]->getRc() )
				{
					line[k] = qRgb(shade, 255, shade);
				}
				else
				{
					line[k] = qRgb(255, shade, 255);
				}
				
				line[k] = qRgb(255, shade, shade);
				
				//line[k] = QColor::fromHsl(240 - 60 * k / image.width(), 128 + 32, 96 + 32).rgb();
			}
			
			for ( int k = getTrackHeight(i) + 1; k < getTrackHeight(i + 1) && k < imageBuffer->height(); k++ )
			{
				memcpy(imageBuffer->scanLine(k), line, sizeof(char) * 4 * imageBuffer->width());
			}
			
			total += (*(*alignment->getTracks())[0])[j]->getLength();
			
			/*if ( i == 0 && j == lcbHover )
			{
				lcbHoverX = (total + (*(*alignment->getTracks())[0])[j]->getLength()) * imageBuffer->width() / refSize;
			}*/
		}
	}
	
	//unsigned int snpMaxCur = snpMax.getValue();
	
	painter.setPen(QColor::fromRgb(qRgb(0, 255, 0)));
	//painter.drawLine(lcbHoverX, frameWidth(), lcbHoverX, height() - frameWidth());
	
	for ( int i = 0; i < getTrackCount(); i++ )
	{
		float childSize = getTrackHeight(i + 1) - getTrackHeight(i);
		int y = (getTrackHeight(i) + getTrackHeight(i + 1)) / 2 + frameWidth();
		int shade;
		
		if ( childSize >= 20 )
		{
			shade = 255;
		}
		else if ( childSize < 2 )
		{
			shade = 0;
		}
		else
		{
			shade = 256 * (childSize - 2) / 18;
		}
		
		if ( getTrackFocus() != -1 && (i == getTrackFocus() || i == getTrackFocus() + 1) )
		{
			painter.setPen(QColor::fromRgba(qRgba(0, 255, 255, 255)));
		}
		else if ( getTrackHover() != -1 && (i == getTrackHover() || i == getTrackHoverEnd() + 1) )
		{
			shade = 255;
			painter.setPen(QColor::fromRgba(qRgba(255, 255, 255, 255)));
		}
		else
		{
			painter.setPen(QColor::fromRgba(qRgba(100, 100, 100, shade)));
		}
		
		if ( shade > 0 )
		{
//			painter.drawLine(frameWidth(), y, width() - frameWidth() - 1, y);
			painter.drawLine(frameWidth() + xOffset, getTrackHeight(i) + frameWidth(), width() - frameWidth() - 1 + xOffset, getTrackHeight(i) + frameWidth());
		}
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
		printf("Clearing...\n");
		imageBuffer->fill(qRgb(0, 0, 0));
		//clearBuffer();
	}
	
	if ( snpsCenter == 0 || ! snpsCenter->ready() )
	{
		return;
	}
	
	for ( int i = getTrackCount() - 1; i >= 0; i-- )
	{
		if ( getTrackHeight(i) < 0 || getTrackHeight(i + 1) >= getHeight() )
		{
			continue;
		}
		
		int bottom = getTrackHeight(i + 1) - 1;
		
		if ( bottom >= getHeight() )
		{
			bottom = getHeight() - 1;
		}
		
		snpsCenter->drawSnps(imageBuffer, getIdByTrack(i), getTrackHeight(i), bottom, posStart, posEnd, getWidth());
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
