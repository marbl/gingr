//
//  BlockViewMain.cpp
//  gavqt
//
//  Created by Brian Ondov on 3/29/13.
//
//

#include "BlockViewMain.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include <QWheelEvent>
#include "BaseBuffer.h"

BlockViewMain::BlockViewMain()
: BlockView()
{
	seq = 0;
	mouseDown = false;
	mouseVelocity = 0;
	zoom = 1;
	offset = 0;
}

BlockViewMain::~BlockViewMain()
{
	if ( seq )
	{
		for ( int i = 0; i < getTrackCount(); i++ )
		{
			delete [ ] seq[i];
		}
		
		delete [] seq;
	}
}

void BlockViewMain::update()
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
			updateMousePosition();
			mouseVelocity *= .85;
		}
	}
	
	int offsetLast = offset;
	
	if ( ! mouseDown && offset )
	{
		offset -= offset * .3;
		
		if ( qAbs(offset) <= 1 )
		{
			offset = 0;
		}
	}
	
	if ( imageBuffer != 0 )
	{
		xOffset = imageBuffer->width() * (float)offset / (posEnd - posStart + 1);
	}
	
	if ( offsetLast != offset )
	{
		setUpdateNeeded();
	}
	
	//	snpMaxCur = snpMaxTarget;
	BlockView::update();
}

void BlockViewMain::setLcbHover(int lcb, float offset)
{
	lcbHover = lcb;
	lcbHoverOffset = offset;
	setUpdateNeeded();
}

void BlockViewMain::setWindow(int start, int end)
{
	posStart = start;
	posEnd = end;
	zoom = (float)refSize / (end - start + 1);
	
	setBufferUpdateNeeded();
	emit windowChanged(posStart, posEnd);
	updateSnps();
}

void BlockViewMain::updateSnpsFinished()
{
	BlockView::updateSnpsFinished();
}

void BlockViewMain::leaveEvent(QEvent * event)
{
	BlockView::leaveEvent(event);
	
	mousePosition = -1;
	emit positionChanged(-1, -1, 0);
}

void BlockViewMain::mouseMoveEvent(QMouseEvent *event)
{
	if ( alignment == 0 )
	{
		return;
	}
	
	TrackListView::mouseMoveEvent(event);
	
	updateMousePosition();
	
	if ( mouseDown )
	{
		setCursor(Qt::ClosedHandCursor);
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

void BlockViewMain::mousePressEvent(QMouseEvent * event)
{
	if ( event->button() == Qt::RightButton )
	{
		//wave = !wave;
		signalToggleSynteny();
	}
	else
	{
		mouseDown = true;
		mouseDownX = getCursorX();
		mouseDownY = getCursorY();
		mouseDownPosition = posStart;
		mouseVelocity = 0;
		clicking = true;
	}
}

void BlockViewMain::mouseReleaseEvent(QMouseEvent *)
{
	if ( alignment == 0 )
	{
		return;
	}
	
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
	else
	{
		setCursor(Qt::ArrowCursor);
	}
}

void BlockViewMain::paintEvent(QPaintEvent * event)
{
	BlockView::paintEvent(event);
	
	float baseWidth = (float)getWidth() / (posEnd - posStart);
	
	if ( baseWidth >= 3 )
	{
		QPainter painter(this);
		
		int x1 = (mousePosition - posStart) * getWidth() / (posEnd - posStart + 1) + frameWidth();
		int x2 = (mousePosition - posStart + 1) * getWidth() / (posEnd - posStart + 1) + frameWidth() - 1;
		painter.setOpacity((baseWidth - 3) / 9);
		
		if ( lightColors )
		{
			painter.setPen(qRgb(0, 0, 0));
		}
		else
		{
			painter.setPen(qRgb(255, 255, 255));
		}
		
		painter.drawLine(x1, frameWidth(), x1, getHeight());
		painter.drawLine(x2, frameWidth(), x2, getHeight());
	}
}

void BlockViewMain::updateBuffer()
{
	BlockView::updateBuffer();
	
	if ( alignment == 0 )
	{
		return;
	}
	
	float baseWidth = (float)getWidth() / (posEnd - posStart + 1);
	
//	if ( snpsCenter->ready() && snpsCenter->getMax() > 1 )
	if ( baseWidth < 1 )
	{
		drawSnps();
	}
	else
	{
	}
	
	if ( snpsCenter->ready() && ! snpsCenter->getSynteny() && snpsCenter->getMax() <= 1 )
	{
		drawSequence();
	}
	
	drawLines();
}

void BlockViewMain::updateTrackCursor()
{
	if ( ! mouseDown )
	{
		TrackListView::updateTrackCursor();
	}
}

void BlockViewMain::updateSnps()
{
	BlockView::updateSnps();
	
	if ( seq )
	{
		for ( int i = 0; i < seqCount; i++ )
		{
			delete [] seq[i];
		}
		
		delete [] seq;
		seq = 0;
	}
	
	int windowSize = posEnd - posStart + 1;
	
	//if ( ! snpsCenter->ready() || snpsCenter->getMax() <= 1 )
	if ( windowSize <= getWidth() )
	{
		seqCount = getTrackCount();
		seq = new char * [seqCount];
		
		for ( int i = 0; i < seqCount; i++ )
		{
			seq[i] = new char[windowSize];
			
			memcpy(seq[i], alignment->getRefSeqGapped() + posStart, windowSize);
		}
	}
}

void BlockViewMain::wheelEvent(QWheelEvent * event)
{
	if ( alignment == 0 )
	{
		return;
	}
	
	mouseVelocity = 0;
	float zoomLast = zoom;
	
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
	
	if ( zoom == zoomLast )
	{
		return;
	}
	
	long int cursor = getCursorX();
	
	int focus = posStart + cursor * (posEnd - posStart + 1) / getWidth();
	
	int size = refSize / zoom;
	int left = cursor * size / getWidth();
	int right = size - left;
	
	posStart = focus - left;
	posEnd = focus + right - 1;
	
	if ( posStart < 0 )
	{
		posStart = 0;
		posEnd = size - 1;
	}
	
	if ( posEnd >= refSize )
	{
		posEnd = refSize;
		posStart = posEnd - size + 1;
	}
	
	setBufferUpdateNeeded();
	emit windowChanged(posStart, posEnd);
	updateSnps();
	setUpdateNeeded();
	//printf("Zoom: %f\t%d\t%d\t%d\n", zoom, posStart, focus, posEnd);
}

void BlockViewMain::drawLines() const
{
	if ( alignment == 0 )
	{
		return;
	}
	
	QPainter painter(imageBuffer);
	
	QLine * lines = new QLine[getTrackCount()];
	int lineCount = 0;
	int minHeight = getTrackHeight(1) - getTrackHeight(0);
	
	for ( int i = 1; i < getTrackCount(); i++ )
	{
		if ( getTrackHeight(i + 1) - getTrackHeight(i) < minHeight )
		{
			minHeight = getTrackHeight(i + 1) - getTrackHeight(i);
		}
	}
	
	QColor colorLine;
	
	if ( lightColors )
	{
		colorLine = qRgb(192, 192, 192);
	}
	else
	{
		colorLine = qRgb(96, 96, 96);
	}
	
	for ( int i = 0; i < getTrackCount(); i++ )
	{
		int childSize = getTrackHeight(i + 1) - getTrackHeight(i);
		bool focus = getTrackFocus() != -1 && (i == getTrackFocus() || i == getTrackFocus() + 1);
		QLine line(xOffset, (int)getTrackHeight(i) + .5, width() - 1 + xOffset, (int)getTrackHeight(i) + .5);
		
		if ( childSize <= minHeight + 1 && ! focus )
		{
			if ( minHeight >= 2 )
			{
				lines[lineCount] = line;
				lineCount++;
			}
		}
		else
		{
			if ( focus )
			{
				if ( lightColors )
				{
					painter.setPen(QColor::fromRgba(qRgba(0, 192, 192, 255)));
				}
				else
				{
					painter.setPen(QColor::fromRgba(qRgba(0, 255, 255, 255)));
				}
			}
			else
			{
				int shade;
				
				if ( childSize >= 20 )
				{
					shade = 255;
				}
				else
				{
					shade = 256 * (childSize - 2) / 18;
				}
				
				painter.setPen(QColor::fromRgba(qRgba(colorLine.red(), colorLine.green(), colorLine.blue(), shade)));
			}
			
			QPen pen;
			pen.setColor(colorLine);
			painter.drawLine(line);
		}
	}
	
	if ( lineCount )
	{
		int shade;
		
		if ( minHeight >= 20 )
		{
			shade = 255;
		}
		else
		{
			shade = 256 * (minHeight - 2) / 18;
		}
		
		painter.setPen(QColor::fromRgba(qRgba(colorLine.red(), colorLine.green(), colorLine.blue(), shade)));
		painter.drawLines(lines, lineCount);
		
	}
}

void BlockViewMain::drawSequence() const
{
	if ( seq == 0 )
	{
		//return;
	}
	
	int imageWidth = imageBuffer->width();
	float baseWidth = (float)imageWidth / (posEnd - posStart + 1);
	
	QPainter painter(imageBuffer);
	
	if ( baseWidth < 1 )
	{
		baseWidth = 1;
	//	return;
	}
	
	int trackHeight = getTrackHeight(1) - getTrackHeight(0);
	
	for ( int i = 1; i < getTrackCount(); i++ )
	{
		if ( getTrackHeight(i + 1) - getTrackHeight(i) < trackHeight )
		{
			trackHeight = getTrackHeight(i + 1) - getTrackHeight(i);
		}
	}
	
	const BaseBuffer * baseBufferRef = new BaseBuffer(baseWidth, trackHeight, lightColors, false);
	const BaseBuffer * baseBufferSnp = new BaseBuffer(baseWidth, trackHeight, lightColors, true);
	
	QImage imageRef(imageWidth, trackHeight + 1, QImage::Format_RGB32);
	QPainter painterRef(&imageRef);
	
	if ( lightColors )
	{
		imageRef.fill(qRgb(240, 240, 240));
	}
	else
	{
		imageRef.fill(qRgb(48, 48, 48));
	}
	
	for ( int i = 0; i < posEnd - posStart + 1; i++ )
	{
		if ( baseWidth <= 1 )
		{
			break;
		}
		
		int bin =
		(float)i /
		float(snpsCenter->getPosEnd() - snpsCenter->getPosStart()) *
		snpsCenter->getBins() +
		float(posStart - snpsCenter->getPosStart()) *
		snpsCenter->getBins() /
		(snpsCenter->getPosEnd() - snpsCenter->getPosStart());
		
		if ( bin < 0 || bin >= snpsCenter->getBins() || snpsCenter->getLcbs()[bin] == 0 )
		{
			continue;
		}
		
		int x = i * imageWidth / (posEnd - posStart + 1);
		
		const QPixmap * charImage = baseBufferRef->image(seq[0][i]);
		
		if ( charImage )
		{
			painterRef.drawPixmap(x, 0, *charImage);
		}
	}
	
	//if ( baseWidth < 2 )
	{
		//painter.setOpacity(baseWidth - 1);
	}
	
	const BaseBuffer * baseBuffersTall[getTrackCount()];
	const BaseBuffer * baseBuffersTallSnp[getTrackCount()];
	
	memset(baseBuffersTall, 0, sizeof(BaseBuffer *) * getTrackCount());
	memset(baseBuffersTallSnp, 0, sizeof(BaseBuffer *) * getTrackCount());
	
	for ( int i = 0; i < getTrackCount(); i++ )
	{
		if ( baseWidth <= 1 )
		{
			break;
		}
		
		if ( getTrackHeight(i + 1) - getTrackHeight(i) > trackHeight + 1)
		{
			if ( baseBuffersTall[i] == 0 )
			{
				baseBuffersTall[i] = new BaseBuffer(baseWidth, getTrackHeight(i + 1) - getTrackHeight(i), lightColors, false);
			}
			
			QImage trackTall(imageWidth, getTrackHeight(i + 1) - getTrackHeight(i) + 1, QImage::Format_RGB32);
			
			if ( lightColors )
			{
				trackTall.fill(qRgb(240, 240, 240));
			}
			else
			{
				trackTall.fill(qRgb(48, 48, 48));
			}
			
			QPainter painterTrackTall(&trackTall);
			
			for ( int j = 0; j < posEnd - posStart + 1; j++ )
			{
				int x = j * imageWidth / (posEnd - posStart + 1);
				int bin =
				(float)j /
				float(snpsCenter->getPosEnd() - snpsCenter->getPosStart()) *
				snpsCenter->getBins() +
				float(posStart - snpsCenter->getPosStart()) *
				snpsCenter->getBins() /
				(snpsCenter->getPosEnd() - snpsCenter->getPosStart());
				
				if ( snpsCenter->getLcbs()[bin] == 0 )
				{
					continue;
				}
				
				const QPixmap * charImage = baseBuffersTall[i]->image(seq[0][j]);
				
				if ( charImage )
				{
					painterTrackTall.drawPixmap(x, 0, *charImage);
//					painter.drawPixmap(x, getTrackHeight(i), *charImage);
				}
			}
			
			painter.drawImage(0, getTrackHeight(i), trackTall);
		}
		else
		{
			painter.drawImage(0, getTrackHeight(i), imageRef);
		}
	}
	
	for ( int i = 0; i < posEnd - posStart + 1; i++ )
	{break;
		int bin =
		(float)i /
		float(snpsCenter->getPosEnd() - snpsCenter->getPosStart()) *
		snpsCenter->getBins() +
		float(posStart - snpsCenter->getPosStart()) *
		snpsCenter->getBins() /
		(snpsCenter->getPosEnd() - snpsCenter->getPosStart());
		
		if ( snpsCenter->getLcbs()[bin] == 0 )
		{
			int x = i * imageWidth / (posEnd - posStart + 1);
			const QPixmap * charImage = baseBufferRef->image(seq[0][i]);
			
			if ( charImage )
			{
//				painter.drawImage(x, getTrackHeight(getTrackById(0)), *charImage);
			}
		}
	}
	
	painter.setOpacity(1);
	
	unsigned int firstSnp = alignment->getNextSnpIndex(posStart);
	
	for ( int i = firstSnp; i < alignment->getSnpCount() && alignment->getSnpPosition(i) >= posStart && alignment->getSnpPosition(i) <= posEnd; i++ )
	{
		for ( int j = 0; j < alignment->getSnpCountByPosition(i); j++ )
		{
			const Alignment::Snp & snp = alignment->getSnpByPosition(i, j);
			bool filter = alignment->filter(snp.filters);
			
			if ( imageBuffer->width() < posEnd - posStart + 1  && ! filter )
			{
				continue;
			}
			
			int x = (alignment->getSnpPosition(i) - posStart) * imageBuffer->width() / (posEnd - posStart + 1);
			
			const QPixmap * charImage;
			const BaseImage * baseImageTall = 0;
			int track = getTrackById(snp.pos);
			
			if ( getTrackHeight(track + 1) - getTrackHeight(track) > trackHeight + 1 )
			{
				if ( filter )
				{
					if ( baseBuffersTallSnp[track] == 0 )
					{
						baseBuffersTallSnp[track] = new BaseBuffer(baseWidth, getTrackHeight(track + 1) - getTrackHeight(track), lightColors, true);
					}
					
					charImage = baseBuffersTallSnp[track]->image(snp.snp);
				}
				else
				{
					charImage = baseBuffersTall[track]->image(snp.snp);
				}
			}
			else
			{
				if ( filter )
				{
					charImage = baseBufferSnp->image(snp.snp);
				}
				else
				{
					charImage = baseBufferRef->image(snp.snp);
				}
			}
			
			if ( charImage )
			{
				painter.setOpacity(1);
				int height = getTrackHeight(track + 1) - getTrackHeight(track) + 1;
				int width = (alignment->getSnpPosition(i) - posStart + 1) * imageWidth / (posEnd - posStart + 1) - x;
				
				if ( width < 1 )
				{
					width = 1;
				}
				
				painter.drawPixmap(QRect(x, getTrackHeight(track), width, height), *charImage, QRect(0, 0, width, height));
			}
			
			if ( baseImageTall )
			{
				delete baseImageTall;
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
	}
	
	delete baseBufferRef;
	delete baseBufferSnp;
}

void BlockViewMain::panTo(int position)
{
	int windowSize = posEnd - posStart + 1;
	
	posStart = position;
	
	int newOffset = 0;
	
	if ( posStart + windowSize > refSize )
	{
		newOffset = -qPow(float(posStart + windowSize - refSize) / windowSize, 2) * windowSize / 4;
		posStart = refSize - windowSize;
	}
	else if ( posStart < 0 )
	{
		newOffset = qPow(float(posStart) / windowSize, 2) * windowSize / 4;
		posStart = 0;
	}
	
	if ( abs(newOffset) > abs(offset) || mouseDown )
	{
		offset = newOffset;
	}
	
	posEnd = posStart + windowSize - 1;
	
	setBufferUpdateNeeded();
	emit windowChanged(posStart, posEnd);
	
	updateSnps();
}

void BlockViewMain::updateMousePosition()
{
	unsigned int focus = posStart + float(posEnd - posStart + 1) * (float(getCursorX()) / getWidth());
	
	if ( getCursorX() >= 0 && getCursorX() < getWidth() )
	{
		mousePosition = focus;
		Alignment::Position ungapped = alignment->getPositionUngapped(focus);
		//		printf("%d:\t%d+%d\n", focus, ungapped.abs, ungapped.gap);
		emit positionChanged(focus, ungapped.abs, ungapped.gap);
	}
	
}
