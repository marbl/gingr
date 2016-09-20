// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

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
	seq = 0;
}

BlockView::~BlockView()
{
	if ( seq )
	{
		delete [] seq;
	}
}

void BlockView::clear()
{
	setTrackFocus(-1);
	alignment = 0;
	setBufferUpdateNeeded();
}

void BlockView::setWindow(int start, int end)
{
	posStart = start;
	posEnd = end;
	
	if ( seq )
	{
		delete [] seq;
		seq = 0;
	}
	
	int windowSize = posEnd - posStart + 1;
	
	//if ( ! snpsCenter->ready() || snpsCenter->getMax() <= 1 )
	if ( windowSize <= getWidth() )
	{
		seq = new char[windowSize];
		memcpy(seq, alignment->getRefSeqGapped() + posStart, windowSize);
	}
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
	
	//emit windowChanged(posStart, posEnd);
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

void BlockView::drawSequence(int trackStart, int trackEnd) const
{
	if ( seq == 0 )
	{
		//return;
	}
	
	bool showGaps = snpsCenter->getShowGaps() & Alignment::SHOW;
	bool showIns = showGaps && snpsCenter->getShowGaps() & Alignment::INSERTIONS;
	bool showDel = showGaps && snpsCenter->getShowGaps() & Alignment::DELETIONS;
	
	int imageWidth = imageBuffer->width();
	float baseWidth = (float)imageWidth / (posEnd - posStart + 1);
	bool drawRef = baseWidth >= 1.;
	unsigned int firstSnp = alignment->getNextSnpIndex(posStart);
	
	QPainter painter(imageBuffer);
	
	if ( baseWidth < 1 )
	{
		baseWidth = 1;
	}
	
	int trackHeight = computeTrackHeight(0);
	
	for ( int i = 1; i < getTrackCount(); i++ )
	{
		if ( computeTrackHeight(i) < trackHeight )
		{
			trackHeight = computeTrackHeight(i);
		}
	}
	
	const BaseBuffer * baseBufferRef = new BaseBuffer(baseWidth, trackHeight, lightColors, false, showIns);
	const BaseBuffer * baseBufferSnp = new BaseBuffer(baseWidth, trackHeight, lightColors, true, showDel);
	const BaseImage gapImage(baseWidth, trackHeight, '-', lightColors, false, showDel);
	
	QImage imageRef(imageWidth, trackHeight + 1, QImage::Format_RGB32);
	
	if ( drawRef )
	{
		drawSequenceRef(&imageRef, baseBufferRef, baseBufferSnp, &gapImage, firstSnp);
	}
	
	const BaseBuffer * baseBuffersTall[getTrackCount()];
	const BaseBuffer * baseBuffersTallSnp[getTrackCount()];
	const BaseImage * gapImagesTall[getTrackCount()];
	//	const BaseImage * gapImage = 0;
	
	memset(baseBuffersTall, 0, sizeof(BaseBuffer *) * getTrackCount());
	memset(baseBuffersTallSnp, 0, sizeof(BaseBuffer *) * getTrackCount());
	memset(gapImagesTall, 0, sizeof(BaseImage *) * getTrackCount());
	
	for ( int i = trackStart; i <= trackEnd; i++ )
	{
		if ( computeTrackHeight(i) > trackHeight + 1)
		{
			if ( baseBuffersTall[i] == 0 )
			{
				baseBuffersTall[i] = new BaseBuffer(baseWidth, computeTrackHeight(i), lightColors, false, showIns);
				baseBuffersTallSnp[i] = new BaseBuffer(baseWidth, computeTrackHeight(i), lightColors, true, showDel);
				gapImagesTall[i] = new BaseImage(baseWidth, computeTrackHeight(i), '-', lightColors, false, showDel);
			}
			
			if ( drawRef )
			{
				QImage trackTall(imageWidth, computeTrackHeight(i) + 1, QImage::Format_RGB32);
				
				drawSequenceRef(&trackTall, baseBuffersTall[i], baseBuffersTallSnp[i], gapImagesTall[i], firstSnp);
				
				painter.drawImage(0, getTrackHeight(i), trackTall);
			}
		}
		else if ( drawRef )
		{
			painter.drawImage(0, getTrackHeight(i), imageRef);
		}
	}
	
	for ( int i = firstSnp; i < alignment->getSnpColumnCount() && alignment->getSnpColumn(i).position >= posStart && alignment->getSnpColumn(i).position <= posEnd; i++ )
	{
		const Alignment::SnpColumn & snpColumn = alignment->getSnpColumn(i);
		char refSnp = snpColumn.ref;
		
		for ( int j = 0; j < alignment->getSnpColumn(i).snps.count(); j++ )
		{
			const Alignment::Snp & snp = alignment->getSnpColumn(i).snps[j];
			bool filter = alignment->filter(snpColumn.filters);
			
			if ( imageBuffer->width() < posEnd - posStart + 1  && ! filter )
			{
				continue;
			}
			
			int x = float(snpColumn.position - posStart) * imageBuffer->width() / (posEnd - posStart + 1);
			
			const QPixmap * charImage = 0;
			int track = getTrackById(snp.track);
			
			if ( track < trackStart || track > trackEnd )
			{
				continue;
			}
			
			if ( computeTrackHeight(track) > trackHeight + 1 )
			{
				if ( filter && snp.snp != refSnp )
				{
					if ( baseBuffersTallSnp[track] == 0 )
					{
						baseBuffersTallSnp[track] = new BaseBuffer(baseWidth, computeTrackHeight(track), lightColors, true, showDel);
					}
					
					charImage = baseBuffersTallSnp[track]->image(snp.snp);
				}
				else if ( snp.snp == '-' && refSnp != '-' && showIns != showDel )
				{
					charImage = gapImagesTall[track];
				}
				else
				{
					if ( baseWidth > 1 )
					{
						charImage = baseBuffersTall[track]->image(snp.snp);
					}
				}
			}
			else
			{
				if ( filter && snp.snp != refSnp )
				{
					charImage = baseBufferSnp->image(snp.snp);
				}
				else if ( snp.snp == '-' && refSnp != '-' && showIns != showDel )
				{
					/*					if ( gapImage == 0 )
					 {
						gapImage = new BaseImage(baseWidth, computeTrackHeight(track), '-', lightColors, false, showDel);
					 }
					 */
					charImage = &gapImage;
				}
				else
				{
					charImage = baseBufferRef->image(snp.snp);
				}
			}
			
			if ( charImage )
			{
				int height = computeTrackHeight(track) + 0;
				int width = (snpColumn.position - posStart + 1) * imageWidth / (posEnd - posStart + 1) - x;
				
				if ( width < 1 )
				{
					width = 1;
				}
				
				painter.drawPixmap(QRect(x, getTrackHeight(track), width, height), *charImage, QRect(0, 0, width, height));
			}
		}
	}
	
	for ( int i = 0; i < getTrackCount(); i++ )
	{
		if ( baseBuffersTall[i] != 0 )
		{
			delete baseBuffersTall[i];
		}
		
		if ( baseBuffersTallSnp[i] != 0 )
		{
			delete baseBuffersTallSnp[i];
		}
		
		if ( gapImagesTall[i] != 0 )
		{
			delete gapImagesTall[i];
		}
	}
	/*
	 if ( gapImage != 0 )
	 {
		delete gapImage;
	 }
	 */
	delete baseBufferRef;
	delete baseBufferSnp;
}

void BlockView::drawSequenceRef(QImage * image, const BaseBuffer * baseBufferRef, const BaseBuffer * baseBufferSnp, const BaseImage * gapImage, int firstSnp) const
{
	QPainter painterRef(image);
	
	int imageWidth = image->width();
	float baseWidth = (float)imageWidth / (posEnd - posStart + 1);
	
	if ( lightColors )
	{
		image->fill(qRgb(240, 240, 240));
	}
	else
	{
		image->fill(qRgb(48, 48, 48));
	}
	
	if ( baseWidth <= 1 )
	{
		if ( lightColors )
		{
			image->fill(qRgb(255, 255, 255));
		}
		else
		{
			image->fill(qRgb(0, 0, 0));
		}
	}
	else
	{
		for ( int i = 0; i < posEnd - posStart + 1; i++ )
		{
			int bin =
			((float)i + .5) /
			float(snpsCenter->getPosEnd() - snpsCenter->getPosStart() + 1) *
			float(snpsCenter->getBins()) +
			float(posStart - snpsCenter->getPosStart()) *
			float(snpsCenter->getBins()) /
			float(snpsCenter->getPosEnd() - snpsCenter->getPosStart() + 1);
			
			if ( bin >= 0 && bin < snpsCenter->getBins() && snpsCenter->getLcbs()[bin] != 0 )
			{
				int x = i * imageWidth / (posEnd - posStart + 1);
				
				const QPixmap * charImage = baseBufferRef->image(seq[i]);
				
				if ( charImage )
				{
					painterRef.drawPixmap(x, 0, *charImage);
				}
			}
		}
	}
	
	//if ( alignment->getTrackReference() != 0 )
	{
		bool showDel = snpsCenter->getShowGaps() & Alignment::SHOW && snpsCenter->getShowGaps() & Alignment::DELETIONS;
		
		for ( int i = firstSnp; i < alignment->getSnpColumnCount() && alignment->getSnpColumn(i).position >= posStart && alignment->getSnpColumn(i).position <= posEnd; i++ )
		{
			const Alignment::SnpColumn & snpColumn = alignment->getSnpColumn(i);
			char ref = alignment->getRefSeqGapped()[snpColumn.position];
			bool filter = alignment->filter(snpColumn.filters);
			
			if ( snpColumn.ref != ref && (filter || ref == '-') )
			{
				int x = float(snpColumn.position - posStart) * imageWidth / (posEnd - posStart + 1);
				const QPixmap * charImage;
				int height = image->height();
				int width = (snpColumn.position - posStart + 1) * imageWidth / (posEnd - posStart + 1) - x;
				
				if ( width < 1 )
				{
					width = 1;
				}
				
				if ( ref == '-' && ! filter && ! showDel ) // relative deletion
				{
					charImage = gapImage;
				}
				else
				{
					charImage = baseBufferSnp->image(ref);
				}
				
				if ( charImage )
				{
					painterRef.drawPixmap(QRect(x, 0, width, height), *charImage, QRect(0, 0, width, height));
				}
			}
		}
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

int BlockView::computeTrackHeight(int track) const
{
	return qFloor(getTrackHeight(track + 1)) - qFloor(getTrackHeight(track));
}

