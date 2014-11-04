// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "BlockViewMain.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include <QWheelEvent>
#include <QMenu>

BlockViewMain::BlockViewMain()
: BlockView()
{
	mouseDown = false;
	mouseVelocity = 0;
	offset = 0;
}

BlockViewMain::~BlockViewMain()
{
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
	BlockView::setWindow(start, end);
	
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
