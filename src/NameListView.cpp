// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "NameListView.h"
#include <QInputDialog>
#include <QMouseEvent>

NameListView::NameListView()
{
	setMouseTracking(true);
	setCursor(Qt::IBeamCursor);
	userWidth = 100;
//	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
	highlight = -1;
	
}

void NameListView::mouseMoveEvent(QMouseEvent * event)
{
	highlight = 0;
	
	while ( heights[highlight + 1] < event->y() - frameWidth() )
	{
		highlight++;
	}
	
	emit highlightTrackChanged(highlight);
//	update();
}

void NameListView::leaveEvent(QEvent *)
{
	if ( ! inDialog )
	{
		highlight = -1;
//		update();
		emit highlightTrackChanged(highlight);
	}
}

void NameListView::mousePressEvent(QMouseEvent * event)
{
	QInputDialog dialog(this);
	
	//int index = (event->y() - frameWidth()) * names->size() / (height() - frameWidth() * 2);
	
	inDialog = true;
	dialog.setModal(true);
	dialog.setLabelText("");
	dialog.setTextValue((*names)[(*order)[highlight]]);
	
	if ( dialog.exec() == QDialog::Accepted )
	{
		(*names)[(*order)[highlight]] = dialog.textValue();
	}
	
	highlight = -1;
	inDialog = false;
	update();
}

void NameListView::paintEvent(QPaintEvent *event)
{
	DrawingArea::paintEvent(event);
	QPainter painter(this);
	
	int x = frameWidth() + 1;
	
//	painter.fillRect(contentsRect(), qRgb(255, 255, 255));
/*	
	if ( highlight > -1 )
	{
		painter.fillRect(frameWidth(), heights[highlight] + frameWidth(), width() - frameWidth() * 2 - 1, heights[highlight + 1] - heights[highlight] + frameWidth(), qRgb(255, 255, 255));
	}
*/	
	for ( int i = 0; i < names->size(); i++ )
	{
		float childSize = heights[i + 1] - heights[i];
		int shade;
		
		if ( childSize >= 10 )
		{
			shade = 255;
		}
		else if ( childSize < 4 )
		{
			shade = 0;
		}
		else
		{
			shade = 256 - 256 * (10 - (heights[i + 1] - heights[i])) / 6;
		}
		
		int yLine = heights[i] + frameWidth();
		
		QRect bg(frameWidth(), heights[i] + frameWidth(), width() - frameWidth() * 2, heights[i + 1] + frameWidth());
		int fontSize = (heights[i + 1] - heights[i]) * .7;
		if ( fontSize > 12 )
		{
			fontSize = 12;
		}
		painter.setFont(QFont(painter.font().family(), fontSize));
//		painter.setPen(QColor::fromHsl(i % 6 * 60, 128, 128).rgb());
		QRgb color = i == highlight ? qRgb(255, 255, 255) : QColor::fromHsl(i * 210 % 360, 50, 220).rgb();
		painter.fillRect(bg, color);
		QPen pen;
		pen.setColor(QColor::fromRgba(qRgba(0, 0, 0, shade)));
		painter.setPen(pen);
		painter.drawText(QRect(x, heights[i] + frameWidth() + (heights[i + 1] - heights[i]) / 5, width() - frameWidth() * 2 - 3, heights[i + 1] + frameWidth()), Qt::AlignRight, (*names)[(*order)[i]]);
		
		if ( i > 0 )
		{
			if ( childSize >= 10 || i == highlight || i == highlight + 1 )
			{
				shade = 255;
			}
			else if ( childSize < 2 )
			{
				shade = 0;
			}
			else
			{
				shade = 256 - 256 * (10 - (heights[i + 1] - heights[i])) / 8;
			}
			
			pen.setColor(QColor::fromRgba(qRgba(0, 0, 0, shade)));
			painter.setPen(pen);
			painter.drawLine(frameWidth(), yLine, width() - frameWidth() - 1, yLine);
		}
	}
}

void NameListView::resizeEvent(QResizeEvent * event)
{
	userWidth = event->size().width();
}

void NameListView::setHighlightTrack(int track)
{
	highlight = track;
	update();
}

QSize NameListView::sizeHint() const
{
	//return QSize(userWidth, 0);
	//return size();
	return DrawingArea::sizeHint();
}

void NameListView::update()
{
	DrawingArea::update();
}
