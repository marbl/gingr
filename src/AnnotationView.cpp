//
//  AnnotationView.cpp
//  gavqt
//
//  Created by Brian Ondov on 2/4/13.
//
//

#include "AnnotationView.h"
#include <QPainter>
#include <QMouseEvent>
#include <QToolTip>

bool annotationLessThan(const Annotation& a, const Annotation& b)
{
	return a.end < b.end;
}

AnnotationView::AnnotationView(QWidget *parent)
: DrawingArea(parent)
{
//	setLineWidth(0);
	alignment = 0;
	annotationStart = 0;
	annotationEnd = 0;
	histogram = 0;
	highlightAnnotation = -1;
	cursorX = -1;
	cursorY = -1;
	focusAnn = -1;
	setMouseTracking(true);
}

AnnotationView::~AnnotationView()
{
	if ( histogram )
	{
		delete [] histogram;
		
		for ( int i = 0; i < rows; i++ )
		{
			delete [] annotationIndeces[i];
		}
		
		delete [] annotationIndeces;
	}
}

void AnnotationView::loadDom(const QDomElement *element)
{
	int i = 0;
	
	for
	(
		QDomElement elementAnn = element->firstChildElement("ann");
		! elementAnn.isNull();
		elementAnn = elementAnn.nextSiblingElement("ann")
	)
	{
		annotations.resize(annotations.size() + 1);
		Annotation * annotation = &annotations[annotations.size() - 1];
		
		annotation->start = elementAnn.attribute("start").toInt();
		annotation->end = elementAnn.attribute("end").toInt();
		annotation->color = QColor::fromHsl(i * 210 % 360, 50, 220).rgb();
		
		if ( elementAnn.hasAttribute("name") )
		{
			annotation->name = elementAnn.attribute("name");
		}
		else
		{
			annotation->name = elementAnn.attribute("locus");
		}
		
		annotation->rc = elementAnn.attribute("strand") != QObject::tr("+");
		i++;
	}
	
	qSort(annotations.begin(), annotations.end(), annotationLessThan);
	setRows(2);
}

void AnnotationView::loadPb(const Harvest::AnnotationList & msg, const Harvest::Reference & msgRef)
{
	annotations.resize(msg.annotations_size());
	
	for ( int i = 0; i < msg.annotations_size(); i++ )
	{
		const Harvest::AnnotationList::Annotation & msgAnn = msg.annotations(i);
		long int refOffset = 0;
		unsigned int refIndex = 0;
		Annotation * annotation = &annotations[i];
		
		while ( msgAnn.sequence() > refIndex )
		{
			refOffset += msgRef.references(refIndex).sequence().length();
			refIndex++;
		}
		
		annotation->start = alignment->getPositionGapped(msgAnn.regions(0).start() + refOffset);
		annotation->end = alignment->getPositionGapped(msgAnn.regions(0).end() + refOffset);
		annotation->color = QColor::fromHsl(i * 210 % 360, 50, 220).rgb();
		
		if ( annotation->start > 3924551 )
		{
			int x = 5;
			x++;
		}
		
		if ( msgAnn.has_name() )
		{
			annotation->name = QString::fromStdString(msgAnn.name());
		}
		
		if ( msgAnn.has_locus() )
		{
			annotation->locus = QString::fromStdString(msgAnn.locus());
		}
		
		if ( msgAnn.has_description() )
		{
			annotation->description = QString::fromStdString(msgAnn.description());
		}
		
		annotation->rc = msgAnn.reverse();
	}
	
	qSort(annotations.begin(), annotations.end(), annotationLessThan);
	setRows(2);
}

void AnnotationView::setAlignment(const Alignment *newAlignment)
{
	alignment = newAlignment;
	start = 0;
	end = alignment->getLength() - 1;
	
}

void AnnotationView::setPosition(int gapped, int, int)
{
	position = gapped;
	updateNeeded = true;
}

void AnnotationView::setWindow(int newStart, int newEnd)
{
	start = newStart;
	end = newEnd;
	updatePosition();
	setAnnotationRange();
	updateNeeded = true;
	setBufferUpdateNeeded();
	
}

void AnnotationView::update()
{
	if ( updateNeeded )
	{
		DrawingArea::update();
		updateNeeded = false;
	}
}

bool AnnotationView::event(QEvent * event)
{
	if (event->type() == QEvent::ToolTip && alignment )
	{
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		
		if ( highlightAnnotation != -1 )
		{
			const Annotation & annotation = annotations[highlightAnnotation];
			QString tooltip;
			
			if ( annotation.name.length() )
			{
				tooltip.append("Name:   \t");
				tooltip.append(annotation.name);
			}
			
			if ( annotation.locus.length() )
			{
				if ( tooltip.length() )
				{
					tooltip.append('\n');
				}
				
				tooltip.append("Locus:  \t");
				tooltip.append(annotation.locus);
			}
			
			if ( annotation.description.length() )
			{
				if ( tooltip.length() )
				{
					tooltip.append('\n');
				}
				
				tooltip.append("Product:\t");
				tooltip.append(annotation.description);
			}
			
			QToolTip::showText(helpEvent->globalPos(), tooltip);
		}
		else
		{
			QToolTip::hideText();
			event->ignore();
		}
		
		return true;
	}
	
	return DrawingArea::event(event);
	
}

void AnnotationView::leaveEvent(QEvent * event)
{
	DrawingArea::leaveEvent(event);
	highlightAnnotation = -1;
	cursorX = -1;
	cursorY = -1;
	emit positionChanged(-1);
	updateNeeded = true;
}

void AnnotationView::mouseMoveEvent(QMouseEvent * event)
{
	DrawingArea::mouseMoveEvent(event);
	
	if ( ! alignment )
	{
		return;
	}
	
	int x = event->pos().x() - frameWidth();
	int y = event->pos().y() - frameWidth();
	
	if ( x >= 0 && x < getWidth() && y >= 0 && y < getHeight() )
	{
		cursorX = x;
		cursorY = y;
		checkHighlight();
	}
	
	updatePosition();
}

void AnnotationView::mousePressEvent(QMouseEvent * event)
{
	DrawingArea::mousePressEvent(event);
	
	if ( highlightAnnotation != -1 )
	{
		const Annotation & annotation = annotations[highlightAnnotation];
		int start = annotation.start - (annotation.end - annotation.start) / 3;
		int end = annotation.end + (annotation.end - annotation.start) / 3;
		
		emit signalWindowTarget(start, end);
	}
	
	focusAnn = highlightAnnotation;
	setBufferUpdateNeeded();
}

void AnnotationView::paintEvent(QPaintEvent * event)
{
	DrawingArea::paintEvent(event);
	
	if ( ! alignment )
	{
		return;
	}
	
	if ( highlightAnnotation != -1 )
	{
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.save();
		painter.translate(frameWidth() + .5, frameWidth() + .5);
		drawAnnotation(highlightAnnotation, &painter, true);
		painter.restore();
	}
}

void AnnotationView::resizeEvent(QResizeEvent *event)
{
	DrawingArea::resizeEvent(event);
	
	if ( alignment )
	{
		renewHistogram();
	}
}

void AnnotationView::updateBuffer()
{
	clearBuffer();
	
	if ( alignment == 0 || imageBuffer == 0 || annotations.size() == 0 )
	{
		return;
	}
	
	if ( width() <= frameWidth() * 2 )
	{
		return;
	}
	
	QPainter painter(imageBuffer);
	
	//painter.setRenderHint(QPainter::Antialiasing);
//	painter.setClipRect(frameWidth(), frameWidth(), width() - frameWidth() * 2, height() - frameWidth() * 2);
	
	drawHistogram();
	
	for ( int i = annotationStart; i <= annotationEnd; i++ )
	{
		drawAnnotationLines(i, &painter);
	}
	
	for ( int i = annotationStart; i <= annotationEnd; i++ )
	{
		drawAnnotation(i, &painter);
	}
	
	checkHighlight();
}

void AnnotationView::wheelEvent(QWheelEvent * event)
{
	DrawingArea::wheelEvent(event);
	emit signalMouseWheel(event->delta());
}

void AnnotationView::checkHighlight()
{
	if ( cursorX == -1 || cursorY == -1 )
	{
		return;
	}
	
	int highlightAnnotationPrev = highlightAnnotation;
	int cursorRow = cursorY * rows / getHeight();
	
	highlightAnnotation = -1;
	
	for ( int i = 0; i < rows; i++ )
	{
		if
			(
			 annotationIndeces[i][cursorX] != -1 &&
			 (
			  highlightAnnotation == -1 ||
			  cursorRow == i
			  )
			 )
		{
			highlightAnnotation = annotationIndeces[i][cursorX];
		}
	}
	
	if ( highlightAnnotation != highlightAnnotationPrev )
	{
		updateNeeded = true;
	}
}

void AnnotationView::drawAnnotation(int index, QPainter * painter, bool highlight)
{
	const Annotation * annotation = &annotations[index];
	QPen pen;
	QColor color = annotation->color;
	bool focus = index == focusAnn;
	
	int x1 = (float)((int)annotation->start - (int)start) * getWidth() / (end - start + 1);
	int x2 = (float)(annotation->end - start + 1) * getWidth() / (end - start + 1) - 1;
	int width = (float)(annotation->end - annotation->start + 1) * getWidth() / (end - start + 1);
	
	if ( ! highlight && ! focus && width < 2 )
	{
		return;
	}
	
	const QString & name = annotation->name.length() ? annotation->name : annotation->locus;
	int bottom = (annotation->row + 1) * getHeight() / rows - 1;
	int top = annotation->row * getHeight() / rows;
	int height = bottom - top + 1;
	int y = (bottom + top) / 2;
	int alpha;
	
	if ( highlight || focus || width > 9 )
	{
		alpha = 255;
	}
	else
	{
		alpha = (width - 1) * 255 / 10;
	}
	
	pen.setWidth(1);
	pen.setCapStyle(Qt::SquareCap);
	painter->setPen(pen);
	QPainterPath path;
	
	color.setAlpha(alpha);
	
	if ( highlight )
	{
		pen.setColor(QColor::fromRgba(qRgba(0, 200, 200, alpha)));
		pen.setCapStyle(Qt::FlatCap);
		pen.setWidth(2);
	}
	else if ( focus )
	{
		pen.setColor(QColor::fromRgba(qRgba(0, 180, 180, alpha)));
		pen.setCapStyle(Qt::FlatCap);
		pen.setWidth(2);
	}
	else
	{
		pen.setColor(QColor::fromRgba(qRgba(0, 0, 0, alpha)));
	}
	
	painter->setPen(pen);
	/*
	alpha = 100;
	color = QColor::fromRgb(qRgb(255, 0, 0));
	pen.setColor(QColor::fromRgba(qRgba(0, 0, 255, alpha)));
	
	path.moveTo(10, 0);
	path.lineTo(20, 10);
	path.lineTo(10, 20);
	path.lineTo(0, 10);
	path.lineTo(10, 0);
	painter->fillPath(path, color);
	painter->strokePath(path, pen);
	return;
	 */
	
	if ( annotation->rc )
	{
		int xb = x1 - y + bottom;
		int xt = x1 - top + y;
		
		if ( xb > x2 ) xb = x2;
		if ( xt > x2 ) xt = x2;
		
		painter->fillRect(xt, top, x2 - xt, height, color);
		path.moveTo(xb, bottom);
		path.lineTo(x1, y);
		path.lineTo(xt, top);
		painter->fillPath(path, color);
		painter->drawLine(xb, bottom, x2, bottom);
		painter->drawLine(xt, top, x2, top);
		painter->drawLine(x2, top, x2, bottom);
		
		if ( highlight )
		{
			pen.setWidth(2);
			painter->setPen(pen);
		}
		
		painter->strokePath(path, pen);
	}
	else
	{
		int xb = x2 + y - bottom;
		int xt = x2 + top - y;
		
		if ( xb < x1 ) xb = x1;
		if ( xt < x1 ) xt = x1;
		
		painter->fillRect(x1 + 1, top, xb - x1, height, color);
		path.moveTo(xt, top);
		path.lineTo(x2, y);
		path.lineTo(xb, bottom);
		painter->fillPath(path, color);
		painter->drawLine(x1, top, x1, bottom);
		painter->drawLine(x1, bottom, xb, bottom);
		painter->drawLine(x1, top, xt, top);
		
		if ( highlight )
		{
			pen.setWidth(2);
			painter->setPen(pen);
		}
		
		painter->strokePath(path, pen);
	}
	
	int shadeText;
	
	if ( height >= 10 )
	{
		shadeText = 255;
	}
	else if ( height < 4 )
	{
		shadeText = 0;
	}
	else
	{
		shadeText = 256 - 256 * (10 - height) / 6;
	}
	
	float fontSize = (height) * .7;
	
	if ( fontSize > 12 )
	{
		fontSize = 12;
	}
	
	pen.setColor(QColor::fromRgba(qRgba(0, 0, 0, shadeText)));
	painter->setPen(pen);
	QFont font;
	font.setPixelSize(fontSize);
	painter->setFont(font);//fontSize));
	
	
	int x1TextMin;
	int x2TextMax;
	int textBufferLeft;
	int textBufferRight;
	int textHeight = painter->fontMetrics().height();
	
	if ( annotation->rc )
	{
		textBufferLeft = textHeight / 2;
		textBufferRight = 0;
	}
	else
	{
		textBufferLeft = 0;
		textBufferRight = textHeight / 2;
	}
	
	if ( x1 + textBufferLeft < 0 )
	{
		x1TextMin = 0;
	}
	else
	{
		x1TextMin = x1 + textBufferLeft;
	}
	
	if ( x2 - textBufferRight > getWidth() )
	{
		x2TextMax = getWidth();
	}
	else
	{
		x2TextMax = x2 - textBufferRight;
	}
	
	int widthText = x2TextMax - x1TextMin > textHeight ? painter->fontMetrics().width(name) + textHeight : textHeight;
	
	int x1Text = (x1 + x2 - widthText) / 2;
	
	if ( x1Text < x1TextMin )
	{
		x1Text = x1TextMin;
	}
	
	int x2Text = x1Text + widthText;
	
	if ( false && highlight )
	{
		x2Text = x1Text + widthText;
	}
	else if ( x2Text > x2TextMax )
	{
		x2Text = x2TextMax;
		
		if ( x2Text > x1TextMin + widthText - 1 )
		{
			x1Text = x2Text - widthText + 1;
		}
		else
		{
			x1Text = x1TextMin;
		}
	}
	/*
	if ( x2 > x2TextMax )
	{
		x2 = x2TextMax;
	}
	
	if ( x2 - x1 + 1 > widthText )
	{
		x2 = x1 + widthText - 1;
	}
	
	if ( x2Text > x2 - fontSize / 2 )
	{
		x2Text = x2 - fontSize / 2;
		x1Text = x2Text - widthText;
	}
	
	if ( x1Text < frameWidth() )
	{
		x1Text = frameWidth();
	}
	
	if ( x2Text > width() - frameWidth() )
	{
		x2Text = width() - frameWidth();
	}
	*/
	
	int flags = Qt::AlignVCenter;
	
	if ( x2Text - x1Text + 1 > widthText - textHeight )
	{
		flags |= Qt::AlignCenter;
	}
	
	if ( x2Text > x1Text )
	{
		if ( highlight )
		{
			painter->translate(-.5, -1.5);
		}
		painter->drawText(QRect(x1Text, top + 1, x2Text - x1Text + 1, height), flags, name);
	}
}

void AnnotationView::drawAnnotationLines(int index, QPainter * painter)
{
	int x1 = (float)((int)annotations[index].start - (int)start) * getWidth() / (end - start + 1);
	int x2 = (float)(annotations[index].end - start + 1) * getWidth() / (end - start + 1) - 1;
	int width = (float)(annotations[index].end - annotations[index].start + 1) * getWidth() / (end - start + 1);
	
	if ( width < 2 )
	{
		return;
	}
	
	int alpha = width > 9 ? 255 : (width) * 255 / 10;
	
	QPen pen;
	pen.setColor(QColor::fromRgba(qRgba(215, 215, 215, alpha)));
	painter->setPen(pen);
	
	painter->drawLine(x1, 0, x1, height());
	painter->drawLine(x2, 0, x2, height());
}

void AnnotationView::drawHistogram()
{
	int radius = 1;
	
	memset(histogram, 0, sizeof(int) * getWidth());
	
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < getWidth(); j++ )
		{
			annotationIndeces[i][j] = -1;
		}
	}
	
	for ( int i = annotationStart; i <= annotationEnd; i++ )
	{
		int x1 = (float)((int)annotations[i].start - (int)start) * getWidth() / (end - start + 1);
		int x2 = (float)(annotations[i].end - start + 1) * getWidth() / (end - start + 1) - 1;
		
		if ( x2 < x1 )
		{
			x2 = x1;
		}
		
		//if ( x2 <= x1 )
		{
			for ( int j = 1; j <= radius; j++ )
			{
				if ( x1 - j > 0 )
				{
					histogram[x1 - j] += radius - j + 1;
				}
				
				if ( x2 + j < getWidth() )
				{
					histogram[x2 + j] += radius - j + 1;
				}
			}
			
			for ( int j = x1; j <= x2; j++ )
			{
				if ( j >= 0 && j < getWidth() )
				{
					histogram[j] += radius + 1;
					annotationIndeces[annotations[i].row][j] = i;
				}
			}
		}
	}
	
	int max = 0;
	
	for ( int i = 0; i < getWidth(); i++ )
	{
		if ( histogram[i] > max )
		{
			max = histogram[i];
		}
	}
	
	if ( max > (radius + 1) * 2 )
	{
		int shade;
		
		if ( max <= (radius + 1) * 5 )
		{
			shade = 30 * (10 - max) / ((radius + 1) * 5 - (radius + 1) * 2);
		}
		else
		{
			shade = 0;
		}
		
		for ( int i = 0; i < getWidth(); i++ )
		{
			for ( int j = getHeight() - getHeight() * histogram[i] / max; j < getHeight(); j++ )
			{
				((QRgb *)imageBuffer->scanLine(j))[i] = qRgb(215 + shade, 235 + shade / 3, 235 + shade / 3);
			}
		}
	}
}

void AnnotationView::renewHistogram()
{
	if ( histogram )
	{
		delete [] histogram;
		
		for ( int i = 0; i < rows; i++ )
		{
			delete [] annotationIndeces[i];
		}
		
		delete [] annotationIndeces;
	}
	
	histogram = new int[getWidth()];
	annotationIndeces = new int * [rows];
	
	for ( int i = 0; i < rows; i++ )
	{
		annotationIndeces[i] = new int[getWidth()];
	}
}

void AnnotationView::setAnnotationRange()
{
	annotationEnd = 0;
	annotationStart = annotations.size();
	
	for ( int i = 0; i < annotations.size(); i++ )
	{
		if ( annotations[i].end >= start && annotations[i].start <= end )
		{
			if ( i < annotationStart )
			{
				annotationStart = i;
			}
			
			if ( i > annotationEnd )
			{
				annotationEnd = i;
			}
		}
	}
}

void AnnotationView::setRows(int newRows)
{
	rows = newRows;
	int rowMax[rows];
	memset(rowMax, 0, rows * sizeof(int));
	
	for ( int i = 0; i < rows; i++ )
	{
		rowMax[i] = 0;
	}

	for ( int i = 0; i < annotations.size(); i++ )
	{
		Annotation & annotation = annotations[i];
		int row = 0;
		
		while ( row < rows && annotation.start <= rowMax[row] )
		{
			row++;
		}
		
		if ( row == rows )
		{
			row = 0;
		}
		
		rowMax[row] = annotation.end;
		annotation.row = row;
	}
	
	renewHistogram();
}

void AnnotationView::updatePosition()
{
	if ( cursorX != -1 )
	{
		unsigned int focus = start + float(end - start + 1) * (float(cursorX) / getWidth());
		
		emit positionChanged(focus);
	}
}
