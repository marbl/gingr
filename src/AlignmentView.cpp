// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include <QTime>
#include "AlignmentView.h"
#include <QMouseEvent>
#include <QtCore>
#include <QPainter>

AlignmentView::AlignmentView(int hueStart, int hueEnd)
{
	alignment = 0;
	highlight = false;
	highlightGradient = false;
	cursorSize = 5;
	
	colors.resize(256);
	
	for ( int i = 0; i < 60; i++ )
	{
		int hue = 240 - i;
		colors[i] = QColor::fromHsl(hue, 160, 127).rgb();
	}
	
	colors[60] = QColor::fromHsl(120, 255, 127).rgb();
	colors[61] = QColor::fromHsl(300, 255, 127).rgb();
	colors[62] = qRgb(200, 200, 200);
	
	initializePalette(hueStart, hueEnd);
	
	setCursor(Qt::BlankCursor);
}

AlignmentView::~AlignmentView()
{
	deletePalette();
	
	for ( int i = 0; i < refByLcb.size(); i++ )
	{
		delete refByLcb[i];
	}
}

void AlignmentView::setAlignment(const Alignment *newAlignment)
{
	alignment = newAlignment;
	
	const RegionVector * trackRef = (*alignment->getTracks())[0]; // TODO: ref idx
	refByLcb.resize(alignment->getLcbCount());
	
	unsigned int total = 0; // total collapsed genome size so far
	
	for ( int i = 0; i < trackRef->size(); i++ )
	{
		const gav::Region * region = (*trackRef)[i];
		refByLcb[region->getLcb()] = new gav::Region(i, total, region->getLength(), region->getRc(), 0);
		
		total += region->getLength();
	}
	
	refSize = total;
	trackViews.resize(alignment->getTracks()->size());
	
	for (int i = 0; i < alignment->getTracks()->size(); i++)
	{
		const RegionVector * track = (*alignment->getTracks())[getIdByTrack(i)];
		trackViews[i].initialize(&refByLcb, track, total, (float)i / trackViews.size(), (float)1 / trackViews.size());
	}
}

void AlignmentView::update(float newProgress)
{
	progress = newProgress;
	tweenNeeded = false;
	cursorSize -= .1;
	
	TrackListView::update();
}

void AlignmentView::setLcbHover(int lcb, float offset)
{
	highlight = true;
	highlightLcb = lcb;
	highlightOffset = offset;
	setUpdateNeeded();
}

void AlignmentView::leaveEvent(QEvent * event)
{
	TrackListView::leaveEvent(event);
	highlight = false;
//	DrawingArea::update();
}

void AlignmentView::mousePressEvent(QMouseEvent * event)
{
	if ( event->button() & Qt::RightButton )
	{
		highlightGradient = ! highlightGradient;
		DrawingArea::update();
	}
	else
	{
		if ( highlight )
		{
			float offset = trackViews[highlightTrack].getLcbOffset(highlightLcb, highlightOffset);
			
			for ( int i = 0; i < trackViews.size(); i++ )
			{
				trackViews[i].alignLcb(highlightLcb, highlightOffset, offset, highlightRc);
			}
			
			tweenNeeded = true;
			progress = 0; // TODO: shouldn't be needed (use signals/slots?)
		}
		
		markLcb = highlightLcb;
		markTrack = highlightTrack;
		markOffset = highlightOffset;
		markRc = highlightRc;
		
		emit signalTrackFocusChange(getTrackHover());
	}
}

void AlignmentView::mouseMoveEvent(QMouseEvent * event)
{
	TrackListView::mouseMoveEvent(event);
	
	float diff = qPow(qPow(event->pos().x() - getCursorX(), 2) + qPow(event->pos().y() - getCursorY(), 2), .5);
	
	cursorX = event->pos().x();
	cursorY = event->pos().y();
	
	cursorSize = 8;// + diff * 1000 / width();
	
	if ( cursorSize > 20 )
	{
		cursorSize = 20;
	}
	
	if
	(
		cursorX >= frameWidth() && cursorX < width() - frameWidth() * 2 &&
		cursorY >= frameWidth() && cursorY < height() - frameWidth() * 2
	)
	{
		highlightTrack = getTrackHover();
		//if ( highlightTrack == trackFocus )
		{
			highlight = true;
	//		float position = (float)x / (width() - frameWidth() * 2);
			int position = cursorX - frameWidth();
			int w = width() - frameWidth() * 2;
			highlightLcb = trackViews[highlightTrack].getLcb(position, w, highlightOffset);
			highlightRc = trackViews[highlightTrack].getRc() != trackViews[highlightTrack].getLcbRc(highlightLcb);
			
			setUpdateNeeded();
			signalLcbHoverChange(highlightLcb, highlightOffset);
		}
		//printf("%d (%d, %d): %d\n", highlightTrack, event->pos().x(), event->pos().y(), highlightLcb);
	}
	
	if ( cursorSize < 10 )
	{
//		cursorSize += 1;
	}
	
	//DrawingArea::update();
}

void AlignmentView::paintEvent(QPaintEvent * event)
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
//	painter.setRenderHints(0);
	
	QImage image(width() - frameWidth() * 2, height() - frameWidth() * 2, QImage::Format_RGB32);
	
//	setColors();
	
	image.setColorCount(62);
	image.setColorTable(colors);
	
	image.fill(qRgb(180, 180, 180));
	
	int * hues = new int[image.width()];
	
	for ( int i = 0; i < image.width(); i++ )
	{
		hues[i] = i * 128 / image.width();
	}
	
	if ( highlight && highlightGradient )
	{
		float radius = .06;
		float offset = wrap(trackViews[highlightTrack].getLcbOffset(highlightLcb, highlightOffset), 0, 1);
		float lcbOffset;
		int lcb = trackViews[highlightTrack].getLcb
		(
			(offset - radius) * image.width(),
			image.width(),
			lcbOffset
		);
		RegionVector * track = (*alignment->getTracks())[getIdByTrack(highlightTrack)];
		
		int i = 0;
		
		// seek to lcb. TODO: Track method?
		//
		while ( (*track)[i]->getLcb() != lcb )
		{
			i++;
		}
		
		int delta = trackViews[highlightTrack].getRc() ? -1 : 1;
		float highlightStart = wrap((*track)[i]->getStartScaled() * delta + trackViews[highlightTrack].getOffset(), 0, 1);
		
		while ( wrap((*track)[i]->getStartScaled() * delta + trackViews[highlightTrack].getOffset(), 0, 1) < offset + radius )
		{
			const gav::Region * region = (*alignment->getLcb((*track)[i]->getLcb()).regions)[0];
			
			int start = region->getStartScaled() * image.width();
			int end = region->getEndScaled() * image.width();
			
			for ( int j = start; j <= end; j++ )
			{
				hues[j] = 60 + (wrap((*track)[i]->getStartScaled() * delta - highlightStart + trackViews[highlightTrack].getOffset(), 0, 1)) * 59 / (radius * 2);
			}
			
			i += delta;
			
			if ( i < 0 )
			{
				i += track->size();break;
			}
			else if ( i == track->size() )
			{
				i = 0;break;
			}
		}
		/*
		for ( int i = wrap(offset - radius, 0, 1) * image.width(); i <= wrap(offset + radius, 0, 1) * image.width() && i < image.width(); i++ )
		{
			int lcb = trackViews[highlightTrack].getLcb
			(
			 i,
			 image.width(),
			 lcbOffset
			 );
			if ( i < 0 )
			{
				i += image.width();
			}
			else if ( i >= image.width() )
			{
				i -= image.width();
			}
			
			int position = (int)(getRefPos(lcb, lcbOffset) * image.width());
			hues[position] = 60 + (i - (offset - radius) * image.width()) * 59 / (2 * radius * image.width());
		}*/
	}
	
	for ( int i = trackViews.size() - 1; i >= 0; i-- )
	{
		trackViews[i].draw(&image, palette, hues, progress, getTrackHeight(i), getTrackHeight(i + 1) - getTrackHeight(i), highlight, highlightLcb, highlightOffset);
	}
	
	delete [] hues;
	
	QTime time = QTime::currentTime();
	
	frames++;
	
	if ( time.second() != secLast )
	{
		//printf("fps: %d\n", frames);
		frames = 0;
	}
	
	secLast = time.second();
	
	painter.drawImage(frameWidth(), frameWidth(), image);
	
	for ( int i = 1; i < trackViews.size(); i++ )
	{
		float childSize = getTrackHeight(i + 1) - getTrackHeight(i);
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
		
		if ( i == getTrackFocus() || i == getTrackFocus() + 1 )
		{
			painter.setPen(QColor::fromRgba(qRgba(255, 255, 255, 255)));
		}
		else if ( i == getTrackHover() || i == getTrackHover() + 1 )
		{
			painter.setPen(QColor::fromRgba(qRgba(180, 180, 180, 255)));
		}
		else
		{
			painter.setPen(QColor::fromRgba(qRgba(0, 0, 0, shade)));
		}
		
		if ( shade > 0 )
		{
			painter.drawLine(frameWidth(), getTrackHeight(i) + frameWidth(), width() - frameWidth() - 1, getTrackHeight(i) + frameWidth());
		}
	}
	
	if ( highlightGradient )
	{
		float radius = .06;
		float offset = wrap(trackViews[highlightTrack].getLcbOffset(highlightLcb, highlightOffset), 0, 1);
		
		int x1 = (offset - radius) * image.width();
		int x2 = (offset + radius) * image.width();
		int y1 = getTrackHeight(highlightTrack);
		int y2 = getTrackHeight(highlightTrack + 1);
		
		QPen pen;
		pen.setWidth(3);
		pen.setColor(Qt::white);
		painter.setPen(pen);
		painter.drawRect(x1 - 1, y1 - 1, x2 - x1 + 2, y2 - y1 + 2);
	}
	
	if ( highlight )
	{
		highlightTrack = getTrackHover();
		int gap = 5;//cursorSize * .3;
		int y = cursorY;//(highlightTrack + .5) * image.height() / trackViews.size() - cursorSize / 20;
		int y1 = getTrackHeight(highlightTrack) - cursorSize / 20 + frameWidth() + 1;
		int y2 = getTrackHeight(highlightTrack + 1) + cursorSize / 20 + frameWidth() + 1;
		
		QPen pen;
		pen.setWidth(2 + cursorSize / 10);
		pen.setColor(QColor::fromHsl(120, 255, 127).rgb());
		painter.setPen(pen);
		painter.drawRect(cursorX - gap + frameWidth(), y1, gap * 2, y2 - y1);
		pen.setColor(Qt::white);
		painter.setPen(pen);
		return;
		painter.drawLine(cursorX - cursorSize, y - cursorSize, cursorX - gap, y - gap);
		painter.drawLine(cursorX - cursorSize, y + cursorSize, cursorX - gap, y + gap);
		painter.drawLine(cursorX + cursorSize, y - cursorSize, cursorX + gap, y - gap);
		painter.drawLine(cursorX + cursorSize, y + cursorSize, cursorX + gap, y + gap);
		
	}
}

// private

void AlignmentView::deletePalette()
{
	for ( int i = 0; i < 64; i++ )
	{
		delete [] palette[i];
	}
	
	delete [] palette;
}

void AlignmentView::drawLcbs(QPainter *painter, int id1, int id2)
{
	RegionVector * trackRef = (*alignment->getTracks())[0];
	RegionVector * track1 = (*alignment->getTracks())[getIdByTrack(id1)];
	RegionVector * track2 = (*alignment->getTracks())[getIdByTrack(id2)];
	
	int indexLast1;
	int indexLast2;
	
	for ( int i = 0; i < track1->size(); i++ )
	{
		int index1;
		int index2;
		
		indexLast1 = index1;
		indexLast2 = index2;
	}
}

float AlignmentView::getRefPos(int lcb, float offset) const
{
	const gav::Region * refLcb = refByLcb[lcb];
	
	if ( refLcb->getRc() )
	{
		return (float)(refLcb->getStart() + refLcb->getLength()) / refSize - offset;
	}
	else
	{
		return (float)refLcb->getStart() / refSize + offset;
	}
}

void AlignmentView::initializePalette(int hueStart, int hueEnd)
{
	palette = new QRgb*[64];
	
	for ( int i = 0; i < 64; i++ )
	{
		palette[i] = new QRgb[256];
		
		int shade;
		
		if ( i < 32 )
		{
			shade = 32 - i * 1.5;
		}
		else
		{
			shade = 128 - i * 1.5;
		}
		
		for ( int j = 0; j < 256; j++ )
		{
			int hue;
			
			if ( j < 128 )
			{
				if ( hueStart == 130 )
				{
					if ( j < 37 )
					{
						hue = 240 - j * 60 / 37;
					}
					else
					{
						hue = 270 + (j - 37) * 40 / 23;
					}
				}
				else
				{
					hue = hueStart + j * (hueEnd - hueStart) / 60;
				}
				palette[i][j] = qRgb(63, 31 + shade + j, 200 + shade);
			}
			else
			{
				hue = j - 60;
			}
			
			//palette[i][j] = QColor::fromHsl(hue, 128 + shade, 96 + shade).rgb();
		}
	}
}

void AlignmentView::setColors()
{
	int imageWidth = width() - frameWidth() * 2;
	
	colors.resize(imageWidth);
	
	for ( int i = 0; i < imageWidth; i++ )
	{
		colors[i] = QColor::fromHsl(240 - i * 60 / imageWidth, 160, 127).rgb();
	}
}
