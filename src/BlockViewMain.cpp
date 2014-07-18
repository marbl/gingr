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
#include <QMenu>

BlockViewMain::BlockViewMain()
: BlockView()
{
	seq = 0;
	mouseDown = false;
	mouseVelocity = 0;
	offset = 0;
}

BlockViewMain::~BlockViewMain()
{
	if ( seq )
	{
		delete [] seq;
	}
}

void BlockViewMain::update()
{
	if ( mouseVelocity && alignment )
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

void BlockViewMain::setReference()
{
	emit signalTrackReference(getIdByTrack(getTrackHover()));
}

void BlockViewMain::setWindow(int start, int end)
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
	
	setBufferUpdateNeeded();
	updateMousePosition();
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
	emit positionChanged(-1);
}

void BlockViewMain::mouseMoveEvent(QMouseEvent *event)
{
	if ( alignment == 0 || ! snpsCenter || ! snpsCenter->ready() )
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
	if ( alignment == 0 )
	{
		return;
	}
	
	if ( event->button() == Qt::RightButton )
	{
		QMenu * menuContext = new QMenu(this);
		QAction * actionReroot = new QAction(tr("Set as variant reference"), this);
		connect(actionReroot, SIGNAL(triggered()), this, SLOT(setReference()));
		menuContext->addAction(actionReroot);
		
		//contextMenu = true;
		emit signalContextMenu(true);
		menuContext->exec(event->globalPos());
		emit signalContextMenu(false);
		//contextMenu = false;
		
		emit signalTrackHoverChange(-1, -1);
		//setUpdateNeeded();
		
		//wave = !wave;
		//signalToggleSynteny();
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
	
	if ( alignment == 0 )
	{
		return;
	}
	
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
	
	if ( alignment == 0 || ! snpsCenter->ready() )
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
	
	if ( snpsCenter->ready() && ! snpsCenter->getSynteny() && (snpsCenter->getMax() == 1 || baseWidth >= 1) )
	{
		drawSequence(0, getTrackCount() - 1);
	}
	else if ( false && snpsCenter->ready() && ! snpsCenter->getSynteny() && snpsCenter->getMaxView() <= 1 )
	{
		drawSequence(snpsCenter->getTrackMin(), snpsCenter->getTrackMax());
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
}

void BlockViewMain::wheelEvent(QWheelEvent * event)
{
	if ( alignment )
	{
		mouseVelocity = 0;
		emit signalMouseWheel(event->delta());
	}
}

int BlockViewMain::computeTrackHeight(int track) const
{
	return qFloor(getTrackHeight(track + 1)) - qFloor(getTrackHeight(track));
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
	
	delete [] lines;
}

void BlockViewMain::drawSequence(int trackStart, int trackEnd) const
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
	
	drawSequenceRef(&imageRef, baseBufferRef, baseBufferSnp, &gapImage, firstSnp);
	
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
			
			QImage trackTall(imageWidth, computeTrackHeight(i) + 1, QImage::Format_RGB32);
			
			drawSequenceRef(&trackTall, baseBuffersTall[i], baseBuffersTallSnp[i], gapImagesTall[i], firstSnp);
			
			painter.drawImage(0, getTrackHeight(i), trackTall);
		}
		else
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

void BlockViewMain::drawSequenceRef(QImage * image, const BaseBuffer * baseBufferRef, const BaseBuffer * baseBufferSnp, const BaseImage * gapImage, int firstSnp) const
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
			if ( baseWidth <= 1 )
			{
				int x = (long long int)i * imageWidth / (posEnd - posStart + 1);
				
				for ( int j = 0; j < image->height(); j++ )
				{
					if ( lightColors )
					{
						((QRgb *)image->scanLine(j))[x] = qRgb(255, 255, 255);
					}
					else
					{
						((QRgb *)image->scanLine(j))[x] = qRgb(0, 0, 0);
					}
				}
			}
			else
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
				int x = (snpColumn.position - posStart) * imageWidth / (posEnd - posStart + 1);
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
	
	offset = 0;
	
	posEnd = posStart + windowSize - 1;
	
	emit windowChanged(posStart, posEnd);
}

void BlockViewMain::updateMousePosition()
{
	unsigned int focus = posStart + float(posEnd - posStart + 1) * (float(getCursorX()) / getWidth());
	
	if ( getCursorX() >= 0 && getCursorX() < getWidth() )
	{
		mousePosition = focus;
		emit positionChanged(focus);
	}
	
}
