//
//  BlockView.cpp
//  gavqt
//
//  Created by Brian Ondov on 11/6/12.
//
//

#include "BlockView.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include <QWheelEvent>
#include <string.h>

BlockView::BlockView()
{
	alignment = 0;
	wave = false;
	
	for ( int i = 0; i < PALETTE_SIZE; i++ )
	{
		float x = (float)i / PALETTE_SIZE;
		
		// gnuplot default heatmap colors (rgbformulae 7,5,15)
		//
		int r = 256 * qSqrt(x);
		int g = 256 * qPow(x, 3);
		int b = 256 * qSin(2 * 3.1415926 * x);
		
		if ( b < 0 )
		{
			b = 0;
		}
		
		snpPalette[i] = qRgb(r, g, b);
	}
	
	snpPalette[PALETTE_SIZE - 1] = qRgb(255, 255, 255);
	imageBuffer = 0;
}

BlockView::~BlockView()
{
	if ( imageBuffer )
	{
		delete imageBuffer;
	}
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
	zoom = 1;
	
	snpsCenter.initialize(alignment);
	snpsLeft.initialize(alignment);
	snpsRight.initialize(alignment);
	
	connect(&snpsCenter, SIGNAL(updated()), this, SLOT(updateSnpsFinished()));
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
	mouseDown = false;
	mouseVelocity = 0;
	
	snpMax.initialize(0, 0);
	snpMaxCur = 0;
	snpMaxTarget = 0;
	
	updateSnps();
}

void BlockView::update()
{
	if ( mouseVelocity )
	{
		if ( mouseDown )
		{
			mouseVelocity *= .5;
		}
		else
		{
			panTo(posStart - (float)mouseVelocity * (posEnd - posStart + 1) / getWidth());
			mouseVelocity *= .85;
		}
	}
	
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
	if ( ! mouseDown && offset )
	{
		offset -= offset * .3;
		
		if ( qAbs(offset) <= 1 )
		{
			offset = 0;
		}
	}
	
//	snpMaxCur = snpMaxTarget;
	TrackListView::update();
}

void BlockView::setLcbHover(int lcb, float offset)
{
	lcbHover = lcb;
	lcbHoverOffset = offset;
	setUpdateNeeded();
}

void BlockView::updateSnpsFinished()
{
	//unsigned int snpMaxTarget = 0;
	snpMaxTarget = 0;
	
	for ( int i = 0; i < getTrackCount(); i++ )
	{
		for ( unsigned int j = 0; j < snpsCenter.getBins(); j++ )
		{
			if ( snpsCenter.getSnps()[i][j] > snpMaxTarget )
			{
				snpMaxTarget = snpsCenter.getSnps()[i][j];
			}
		}
	}
	
	snpMaxTarget = snpsCenter.getSnpMax();
	snpMax.setTarget(snpMaxTarget, (snpMaxTarget - snpMax.getEnd()) / snpMax.getEnd() < .2);
	snpMaxTimer.initialize(320);
	
	if ( false && timeZoomIn.msecsTo(QDateTime::currentDateTime()) > 50 )
	{
		printf("updating left: %d\t%d\n", posStart * 2 - posEnd - 1, posStart - 1);
		snpsLeft.update(posStart * 2 - posEnd - 1, posStart - 1, width() - frameWidth() * 2);
		printf("updating right: %d\t%d\n", posEnd + 1, 2 * posEnd - posStart + 1);
		snpsRight.update(posEnd + 1, 2 * posEnd - posStart + 1, width() - frameWidth() * 2);
	}
	
	setUpdateNeeded();
}

void BlockView::updateSnpsNeeded()
{
	updateSnps();
}

void BlockView::mouseMoveEvent(QMouseEvent *event)
{
	TrackListView::mouseMoveEvent(event);
	
	unsigned int focus = posStart + float(posEnd - posStart) * (float(getCursorX()) / getWidth());
	
	if ( getCursorX() >= 0 && getCursorX() < getWidth() )
	{
		Alignment::Position ungapped = alignment->getPositionUngapped(focus);
//		printf("%d:\t%d+%d\n", focus, ungapped.abs, ungapped.gap);
		emit positionChanged(focus, ungapped.abs, ungapped.gap);
	}
	
	if ( mouseDown )
	{
		panTo(mouseDownPosition + float(mouseDownX - getCursorX()) * (posEnd - posStart + 1) / getWidth());
		mouseVelocity = getCursorX() - mouseXLast;
		//printf("cur: %d\tlast: %d\t$vel: %d\n", getCursorX(), mouseXLast, mouseVelocity);
	}
	
	unsigned int total = 0;
	int w = width() - frameWidth() * 2;
	
	for ( int i = 0; i < alignment->getLcbCount(); i++ )
	{
		unsigned int length = (*(*alignment->getTracks())[0])[i]->getLength();
		
		if ( float(total + length) / refSize * w >= getCursorX() )
		{
			lcbHover = i;
			lcbHoverOffset = getCursorX() / w;
			signalLcbHoverChange(lcbHover, lcbHoverOffset);
			break;
		}
		
		total += length;
	}
	
	mouseXLast = getCursorX();
	clicking = false;
	
	setUpdateNeeded();
}

void BlockView::mousePressEvent(QMouseEvent * event)
{
	mouseDown = true;
	mouseDownX = getCursorX();
	mouseDownY = getCursorY();
	mouseDownPosition = posStart;
	mouseVelocity = 0;
	clicking = true;
	
	if ( event->button() == Qt::RightButton )
	{
		wave = !wave;
	}
}

void BlockView::mouseReleaseEvent(QMouseEvent *)
{
	mouseDown = false;
	
	if ( clicking )
	{
		if ( getTrackHover() == getTrackFocus() )
		{
			signalTrackFocusChange(-1);
		}
		else
		{
			signalTrackFocusChange(getTrackHover());
		}
	}
}

void BlockView::updateBuffer()
{
	drawSnps();
}

void BlockView::wheelEvent(QWheelEvent * event)
{
	if ( event->delta() > 0 )
	{
		timeZoomIn = QDateTime::currentDateTime();
	}
	else
	{
		timeZoomOut = QDateTime::currentDateTime();
	}
	
	float zoomFactor = 1 + qAbs((float)event->delta()) / 400;
	
	if ( event->delta() > 0 )
	{
		zoom *= zoomFactor;
	}
	else
	{
		zoom /= zoomFactor;
	}
	
	if ( zoom < 1 )
	{
		zoom = 1;
	}
	
	float zoomMax = 12. * refSize / getWidth();
	
	if ( zoom > zoomMax )
	{
		zoom = zoomMax;
	}
	
	long int cursor = getCursorX();
	
	int focus = posStart + cursor * (posEnd - posStart + 1) / getWidth();
	
	int size = refSize / zoom;
	int left = cursor * size / getWidth();
	
//	if ( left + right > refSize )
	if ( left > focus )
	{
		focus = left;
	}
	
	int right = size - left;
	
	posStart = focus - left;
	posEnd = focus + right - 1;
	
	emit windowChanged(posStart, posEnd);
	updateSnps();
	setUpdateNeeded();
	printf("Zoom: %f\t%d\t%d\t%d\n", zoom, posStart, focus, posEnd);
}

inline float absf(float f) {return f > 0 ? f : -f;}

void BlockView::paintEvent(QPaintEvent * event)
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
	
	QPainter painter(this);
	imageBuffer->fill(qRgb(255, 255, 255));
	int lcbHoverX = 0;
//	unsigned int totals[getTrackCount()][image.width()];
	
	for ( int i = getTrackCount() - 1; i >= 0; i-- )
	{
		unsigned int total = 0;
//		memset(totals[i], 0, image.width() * sizeof(unsigned int));
		
		for ( unsigned int j = 0; j < SNP_WINDOW / zoom; j++ )
		{
//			totals[i][(int)(j * image.width() * zoom / SNP_WINDOW)] += snpsMax[i][j];
		}
		
		QRgb * line = (QRgb *)imageBuffer->scanLine(getTrackHeight(i));
		
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
			
			if ( i == 0 && j == lcbHover )
			{
				lcbHoverX = (total + (*(*alignment->getTracks())[0])[j]->getLength()) * imageBuffer->width() / refSize;
			}
		}
	}
	
	//unsigned int snpMaxCur = snpMax.getValue();
	
	updateBuffer();
	
	int xOffset = imageBuffer->width() * (float)offset / (posEnd - posStart + 1);
	painter.drawImage(frameWidth() + xOffset, frameWidth(), *imageBuffer);
	
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
			painter.setPen(QColor::fromRgba(qRgba(255, 255, 255, 255)));
		}
		else if ( getTrackHover() != -1 && (i == getTrackHover() || i == getTrackHoverEnd() + 1) )
		{
			painter.setPen(QColor::fromRgba(qRgba(255, 255, 255, 255)));
		}
		else
		{
			painter.setPen(QColor::fromRgba(qRgba(0, 0, 0, shade)));
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
	
	if ( imageBuffer )
	{
		delete imageBuffer;
	}
	
	imageBuffer = new QImage(getWidth(), getHeight(), QImage::Format_RGB32);
	
	if ( alignment )
	{
		updateSnps();
	}
}

void BlockView::drawSnps() const
{
	int windowSize = posEnd - posStart + 1;
	int imageWidth = imageBuffer->width();
	int imageHeight = imageBuffer->height();
	
	for ( int i = getTrackCount() - 1; i >= 0; i-- )
	{
		if ( getTrackHeight(i) < 0 || getTrackHeight(i + 1) >= getHeight() )
		{
			continue;
		}
		
		if ( true )//|| baseWidth < 1 )
		{
			int radius1;
			int radius2 = (getTrackHeight(i + 1) - getTrackHeight(i)) / 2;
			
			int height = getTrackHeight(i + 1) - getTrackHeight(i);
			
			if ( radius2 * 2 != getTrackHeight(i + 1) - getTrackHeight(i) )
			{
				radius1 = radius2 + 1;
			}
			else
			{
				radius1 = radius2;
			}
			
			//if ( snpsCur )
			{
				for ( int j = 0; j < imageWidth; j++ )
				{
					int bin =
					j * (float)windowSize /
					float(snpsCenter.getPosEnd() - snpsCenter.getPosStart()) *
					snpsCenter.getBins() /
					imageWidth +
					float(posStart - snpsCenter.getPosStart()) *
					snpsCenter.getBins() /
					(snpsCenter.getPosEnd() - snpsCenter.getPosStart());
					
					const SnpBuffer * snpsCur;
					
					if ( bin < 0 || bin >= snpsCenter.getBins() )
					{
						continue;
					}
					
					if ( bin < 0 )
					{
						snpsCur = &snpsLeft;
					}
					else if ( bin > snpsCenter.getBins() )
					{
						snpsCur = &snpsRight;
					}
					else
					{
						snpsCur = &snpsCenter;
					}
					snpsCur = &snpsCenter;
					
					int snps = snpMaxCur > 0 ? snpsCur->getSnps()[getIdByTrack(i)][bin] : 0;
					
					if ( wave )
					{
						float breakFactor = (float)(snpsCenter.getLcbs()[bin] * snpsCenter.getBins()) / windowSize;
						int breakShade = 220;
						int shade = breakShade + 35 * breakFactor;
						
						for ( int k = 0; k + radius2 < height && k + getTrackHeight(i) < imageHeight; k++ )
						{
							((QRgb *)imageBuffer->scanLine(getTrackHeight(i) + radius2 + k))[j] = qRgb(shade, shade, shade);
						}
						int peak = snps * (radius1) / snpMaxCur;
						int remainder = snps - peak * snpMaxCur / radius1;
						
						if ( peak > height )
						{
							peak = height;
						}
						
						//for ( int k = (height - peak) / 2; k < peak + (height - peak) / 2 && k + getTrackHeight(i) < image.height(); k++ )
						for ( int k = 0; k <= peak && k + radius2 < height && k + getTrackHeight(i) < imageHeight; k++ )
						{
							if ( k == peak )
							{
								int shade = 255 - remainder * radius1 * 255 / snpMaxCur;
								int gb = shade * (220 + 35 * (breakFactor)) / 255;
								
								((QRgb *)imageBuffer->scanLine(getTrackHeight(i) + radius2 + k))[j] = qRgb(128 + shade / 2 * (93 + 35 * (1. - breakFactor)) / 128, gb, gb);
							}
							else
							{
								((QRgb *)imageBuffer->scanLine(getTrackHeight(i) + radius2 + k))[j] = qRgb(128, 0, 0);
							}
						}
					}
					else
					{
						int shade = snpMaxCur ? 255 - snps * 256 / snpMaxCur : 255;
						float breakFactor = 1. - (float)(snpsCenter.getLcbs()[j] * snpsCenter.getBins()) / windowSize;
						int breakShade = 220;
						int gb = shade * (220 + 35 * (1. - breakFactor)) / 255;
						//((QRgb *)image.scanLine(getTrackHeight(i)))[j] = qRgb((128 + shade / 2) * (1. - breakFactor) + breakShade * breakFactor, shade * (1. - breakFactor) + breakShade * breakFactor, shade * (1. - breakFactor) + breakShade * breakFactor);
						//((QRgb *)image.scanLine(getTrackHeight(i)))[j] = qRgb(128 + shade / 2 * (93 + 35 * (1. - breakFactor)) / 128, gb, gb);
						//shade = j;
						//shade %= 256;
						
						shade = snpMaxCur ? snps * PALETTE_SIZE / snpMaxCur : PALETTE_SIZE - 1;
						
						if ( shade >= PALETTE_SIZE )
						{
							shade = PALETTE_SIZE - 1;
						}
						if ( shade < 0 )
						{
							shade = 0;
						}
						
						if ( snpsCenter.getLcbs()[bin] == 0 )
						{
							((QRgb *)imageBuffer->scanLine(getTrackHeight(i)))[j] = qRgb(80, 80, 80);
						}
						else
						{
							((QRgb *)imageBuffer->scanLine(getTrackHeight(i)))[j] = snpPalette[shade];
						}
					}
				}
			}
			
			if ( wave )
			{
				for ( int j = 0; j < radius2; j++ )
				{
					memcpy(imageBuffer->scanLine(getTrackHeight(i) + j), imageBuffer->scanLine(getTrackHeight(i + 1) - j - 1), sizeof(char) * 4 * imageWidth);
				}
			}
			else
			{
				for ( int j = getTrackHeight(i) + 1; j < getTrackHeight(i + 1) && j < imageHeight; j++ )
				{
					memcpy(imageBuffer->scanLine(j), imageBuffer->scanLine(getTrackHeight(i)), sizeof(char) * 4 * imageWidth);
				}
			}
		}
	}
}

void BlockView::panTo(int position)
{
	int windowSize = posEnd - posStart + 1;
	
	posStart = position;
	
	if ( posStart + windowSize > refSize )
	{
		offset = -qPow(float(posStart + windowSize - refSize) / windowSize, 2) * windowSize / 4;
		posStart = refSize - windowSize;
	}
	else if ( posStart < 0 )
	{
		offset = qPow(float(posStart) / windowSize, 2) * windowSize / 4;
		posStart = 0;
	}
	
	posEnd = posStart + windowSize - 1;
	
	emit windowChanged(posStart, posEnd);
	
	updateSnps();
}

void BlockView::updateSnps()
{
	if ( width() <= frameWidth() * 2 )
	{
		return;
	}
	
//	printf("updating cent: %d\t%d\n", posStart, posEnd);
//	snpsCenter.update(posStart, posEnd, width() - frameWidth() * 2);
	snpsCenter.update(posStart * 2 - posEnd - 1, 2 * posEnd - posStart + 1, 3 * (width() - frameWidth() * 2));
	
	if ( false && timeZoomOut.msecsTo(QDateTime::currentDateTime()) < 50 )
	{
		printf("updating left: %d\t%d\n", posStart * 2 - posEnd - 1, posStart - 1);
		snpsLeft.update(posStart * 2 - posEnd - 1, posStart - 1, width() - frameWidth() * 2);
		printf("updating right: %d\t%d\n", posEnd + 1, 2 * posEnd - posStart + 1);
		snpsRight.update(posEnd + 1, 2 * posEnd - posStart + 1, width() - frameWidth() * 2);
	}
}
