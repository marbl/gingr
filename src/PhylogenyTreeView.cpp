//
//  PhylogenyTreeView.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "PhylogenyTreeView.h"
#include <stdlib.h>
#include <QtCore>

inline float absf(float x) {return x > 0 ? x : -x;}

PhylogenyTreeView::PhylogenyTreeView()
{
	phylogenyTree = 0;
	penDark.setColor(qRgb(0, 0, 0));
	penLight.setColor(qRgb(200, 200, 200));
//	buffer = 0;
	nameBuffers = 0;
	nameBufferCount = 0;
	nodeViews = 0;
	highlightNode = 0;
	focusNode = 0;
}

PhylogenyTreeView::~PhylogenyTreeView()
{
	if ( nameBuffers )
	{
		for ( int i = 0; i < nameBufferCount; i++ )
		{
			delete nameBuffers[i];
		}
		
		delete [] nameBuffers;
	}
	
	if ( nodeViews )
	{
		delete [] nodeViews;
	}
}

void PhylogenyTreeView::handleTrackHeightChange(const TrackListView * focus)
{
	TrackListView::handleTrackHeightChange(focus);
	redrawNeeded = true;
}

void PhylogenyTreeView::setNames(QVector<QString> *newNames)
{
	names = newNames;
	
	if ( nameBuffers )
	{
		for ( int i = 0; i < nameBufferCount; i++ )
		{
			delete nameBuffers[i];
		}
		
		delete [] nameBuffers;
	}
	
	nameBufferCount = names->size();
	nameBuffers = new QImage * [nameBufferCount];
	
	QFont defaultFont;
	defaultFont.setPixelSize(20);
	QFontMetrics fm(defaultFont);
	
	for ( int i = 0; i < nameBufferCount; i++ )
	{
		int width = fm.width((*names)[i]);
		fontHeight = fm.height();
		nameBuffers[i] = new QImage(width, fontHeight, QImage::Format_ARGB32);
		QPainter painter(nameBuffers[i]);
		painter.setFont(defaultFont);
		nameBuffers[i]->fill(qRgba(0, 0, 0, 0));
		painter.drawText(nameBuffers[i]->rect(), (*names)[i]);
	}
}

void PhylogenyTreeView::setPhylogenyTree(const PhylogenyTree * newTree)
{
	phylogenyTree = newTree;
	
	if ( nodeViews )
	{
		delete [] nodeViews;
	}
	
	nodeViews = new PhylogenyNodeView[phylogenyTree->getNodeCount()];
	redrawNeeded = true;
	
	for ( int i = 0; i < phylogenyTree->getNodeCount(); i++ )
	{
		nodeViews[i].search = false;
	}
	
	setWindow(phylogenyTree->getRoot());
	setWindow(phylogenyTree->getRoot());
	focusNode = phylogenyTree->getRoot();
}

void PhylogenyTreeView::setTrackFocus(int track)
{
	xFactorStart = xFactorEnd;
	xOffsetStart = xOffsetEnd;
	TrackListView::setTrackFocus(track);
}

void PhylogenyTreeView::setTrackHover(int track, int trackEnd)
{
	TrackListView::setTrackHover(track, trackEnd);
	
	if ( track == trackEnd )
	{
		if ( track == -1 )
		{
			highlightNode = 0;
		}
		else
		{
			highlightNode = phylogenyTree->getLeaf(track);
		}
	}
}

float PhylogenyTreeView::getHighlight(const PhylogenyNode *, float highlight, bool) const
{
	return highlight;
}

void PhylogenyTreeView::leaveEvent(QEvent *event)
{
	TrackListView::leaveEvent(event);
	
	highlightNode = 0;
}

void PhylogenyTreeView::paintEvent(QPaintEvent *event)
{
	if ( phylogenyTree == 0 )
	{
		return;
	}
	
	xFactor = zoomLerp(xFactorStart, xFactorEnd);
	xOffset = zoomLerp(xOffsetStart, xOffsetEnd);
	
	if ( redrawNeeded || bufferHighlight )
	{
		setBufferUpdateNeeded();
	}
	
	TrackListView::paintEvent(event);
//	painter.drawImage(frameWidth(), frameWidth(), *buffer);
	
	if ( ! redrawNeeded && highlightNode )
	{
		QPainter painter(this);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.save();
		painter.translate(1, 1);
		drawNode(&painter, highlightNode, true);
		
		if ( getTrackFocus() != -1 )
		{
			drawNode(&painter, phylogenyTree->getLeaf(getTrackFocus()), true);
		}
		painter.restore();
	}
	
	redrawNeeded = false;
}

void PhylogenyTreeView::resizeEvent(QResizeEvent * event)
{
	TrackListView::resizeEvent(event);
	radius = height() / 10;
	/*
	if ( buffer )
	{
		delete buffer;
	}
	
	buffer = new QImage(getWidth(), getHeight(), QImage::Format_RGB32);
	redrawNeeded = true;*/
}

void PhylogenyTreeView::setWindow(const PhylogenyNode *node)
{
	int nameWidth = 200;
	
	if ( nameWidth > 200 )
	{
		nameWidth = 200;
	}
	
	if ( nameWidth > getWidth() / 3 )
	{
		nameWidth = getWidth() / 3;
	}
	
	xFactorStart = xFactorEnd;
	xOffsetStart = xOffsetEnd;
	
	float leafSize = (float)getHeight() / node->getLeafCount();
	xFactorEnd = (getWidth() - (leafSize >= 5 || getTrackFocus() != -1 ? nameWidth : 0)) / (maxVisibleDepth(node, leafSize) - node->getDepth());
	xOffsetEnd = -xFactorEnd * node->getDepth();
}

void PhylogenyTreeView::updateBuffer()
{
	updateNodeViews(phylogenyTree->getRoot());
	QPainter painterBuffer(imageBuffer);
	painterBuffer.setRenderHint(QPainter::SmoothPixmapTransform);
	painterBuffer.setRenderHint(QPainter::Antialiasing);
	painterBuffer.fillRect(0, 0, getWidth(), getHeight(), qRgb(235, 235, 235));
	//clearBuffer();
	drawNode(&painterBuffer, phylogenyTree->getRoot(), redrawNeeded);
	bufferHighlight = redrawNeeded && highlightNode;
	
	if ( getTrackFocus() != -1 )
	{
		drawNode(&painterBuffer, phylogenyTree->getLeaf(getTrackFocus()), true);
	}
}

void PhylogenyTreeView::drawLine(QPainter * painter, float x1, float y1, float x2, float y2, float weight, QColor color) const
{
	if ( weight == 0 )
	{
		return;
	}
	
	if ( x1 != x2 && y1 != y2 )
	{
		if ( x1 < x2 )
		{
			x1 += .25;
			x2 -= .75;
		}
		else
		{
			x1 -= .75;
			x2 += .25;
		}
		
		if ( y1 < y2 )
		{
			y1 += .25;
			y2 -= .75;
		}
		else
		{
			y1 -= .75;
			y2 += .25;
		}
	}
	
	QPen pen;
	pen.setColor(qRgb(color.red(), color.green(), color.blue()));
	pen.setWidthF(weight * 1.5);
	painter->setPen(pen);
	painter->drawLine(QLineF((float)x1 + .5, (float)y1 + .5, (float)x2 + .5, (float)y2 + .5));
	
	return;
	
	if ( weight > .3 )
	{
		QPen pen;
		
		if ( x1 == x2 || y1 == y2 )
		{
			pen.setWidth(3);
			pen.setCapStyle(Qt::FlatCap);
		}
		else
		{
			pen.setWidth(2);
			pen.setCapStyle(Qt::SquareCap);
		}
		
		pen.setColor(QColor::fromRgba(qRgba(color.red(), color.green(), color.blue(), 255 * ((weight - .3) / 3 + .1))));
		painter->setPen(pen);
		painter->drawLine(x1, y1, x2, y2);
		pen.setCapStyle(Qt::SquareCap);
		pen.setWidth(1);
		pen.setColor(color);
		painter->setPen(pen);
		painter->drawLine(x1, y1, x2, y2);
	}
	else
	{
		QPen pen;
		
		if ( x1 == x2 || y1 == y2 )
		{
			pen.setWidth(3);
			pen.setCapStyle(Qt::FlatCap);
		}
		else
		{
			pen.setWidth(2);
			pen.setCapStyle(Qt::SquareCap);
		}
		
		pen.setColor(QColor::fromRgba(qRgba(color.red(), color.green(), color.blue(), 255 * (weight) / 3)));
		painter->setPen(pen);
		painter->drawLine(x1, y1, x2, y2);
		pen.setCapStyle(Qt::SquareCap);
		pen.setWidth(1);
		pen.setColor(QColor::fromRgba(qRgba(color.red(), color.green(), color.blue(), 255 * (weight / .3))));
		painter->setPen(pen);
		painter->drawLine(x1, y1, x2, y2);
	}
}


void PhylogenyTreeView::drawNode(QPainter * painter, const PhylogenyNode *node, bool drawHighlight, float highlight, float weightTop, float weightBottom) const
{
	PhylogenyNodeView & nodeView = nodeViews[node->getId()];
	
	float childSize = getTrackHeight(node->getLeafMax() + 1) - getTrackHeight(node->getLeafMin());
	int x = nodeView.x;
	
	highlight = getHighlight(node, highlight, drawHighlight);
	
	float weight;
	float maxHeight = 150;
	float minHeight = 2;
	
	if
	(
		childSize >= maxHeight ||
		node == highlightNode
		//(getTrackFocus() >= nodeView.leafMin && getTrackFocus() <= nodeView.leafMax) ||
		//(getTrackHover() == nodeView.leafMin || getTrackHoverEnd() == nodeView.leafMax)
	)
	{
		weight = 1;
	}
	else if ( childSize < minHeight )
	{
		weight = 0;
	}
	else
	{
		weight = 1 - (maxHeight - childSize) / (maxHeight - minHeight);
	}
	
	QPen pen;
	
	int bootShade = 200;//255. * (1. - node->getBootstrap());
	
	if ( node->getBootstrap() == 0 || node->getBootstrap() > .5 )
	{
		bootShade = 0;
	}
	
	if( node->getChildrenCount() && ! node->getCollapse() )
	{
		int childMinY = nodeViews[node->getChild(0)->getId()].y;
		int childMaxY = nodeViews[node->getChild(node->getChildrenCount() - 1)->getId()].y;
		
//		painter->fillRect(x + 1, childMinY, getWidth() - x - 1, childMaxY - childMinY, qRgb(245, bootShade, bootShade));
		
		if ( highlight )
		{
//			painter->setOpacity(1 - getZoomProgress());
			painter->fillRect(x + 1, childMinY, getWidth() - x - 1, childMaxY - childMinY, highlightColor(highlight));
//			painter->setOpacity(1);
		}
		
		//pen.setColor(QColor::fromRgba(qRgba(bootShade, 0, 0, 255 * maxf(weight, weightTop))));
		//painter->setPen(pen);
//		painter->drawLine(x, childMinY, x, nodeView.y);
		//drawLine(painter, x, childMinY, x, nodeView.y, maxf(weight, weightTop), qRgb(bootShade, 0, 0));
		
		//pen.setColor(QColor::fromRgba(qRgba(bootShade, 0, 0, 255 * maxf(weight, weightBottom))));
		//painter->setPen(pen);
//		painter->drawLine(x, nodeView.y, x, childMaxY);
		//drawLine(painter, x, childMaxY, x, nodeView.y, maxf(weight, weightBottom), qRgb(bootShade, 0, 0));
	}
	else
	{
		drawNodeLeaf(painter, node, highlight, maxf(weight, weightTop), maxf(weight, weightBottom));
	}
	
	for( int i = 0; i < node->getChildrenCount(); i++ )
	{
		float weightChildTop = 0;
		float weightChildBottom = 0;
		
		if ( i == 0 )
		{
			weightChildTop = maxf(weight, weightTop);
		}
		
		if ( i == node->getChildrenCount() - 1 )
		{
			weightChildBottom = maxf(weight, weightBottom);
		}
		
		drawNode(painter, node->getChild(i), drawHighlight, highlight, weightChildTop, weightChildBottom);
	}
	
	if( node->getChildrenCount() && ! node->getCollapse() )
	{
		int childMinY = nodeViews[node->getChild(0)->getId()].y;
		int childMaxY = nodeViews[node->getChild(node->getChildrenCount() - 1)->getId()].y;
		
		drawLine(painter, x, childMinY, x, nodeView.y, maxf(weight, weightTop), qRgb(bootShade, 0, 0));
		drawLine(painter, x, childMaxY, x, nodeView.y, maxf(weight, weightBottom), qRgb(bootShade, 0, 0));
	}
	else
	{
		drawNodeLeaf(painter, node, highlight, maxf(weight, weightTop), maxf(weight, weightBottom));
	}
	int y = nodeView.y;
	
	float maxWeight = maxf(maxf(weightTop, weightBottom), weight);
	
	pen.setColor(QColor::fromRgba(qRgba(bootShade, 0, 0, 255 * maxWeight)));
	painter->setPen(pen);
	
	int xLeft = xOffset + xFactor * (node->getDepth() - node->getDistance()) + 1;
	
	if ( xLeft < x )
	{
//		painter->drawLine(xLeft, y, x, y);
		drawLine(painter, xLeft, y, x, y, maxWeight, qRgb(bootShade, 0, 0));
	}
	
	//float dist = qSqrt(qPow(getCursorX() - x, 2) + qPow(getCursorY() - y, 2));
	
	int radius = 6;
	if ( false && ! highlightNode && node->getChildrenCount() && getCursorX() >= x - radius && getCursorX() <= x + radius && getCursorY() >= y - radius && getCursorY() <= y + radius )//dist < 20)
	{
		int shadeBox = 255;//dist < 10 ? 255 : 255 - 256 * (dist - 10) / 10;
		
//		highlightNode = node;
		painter->fillRect(x - radius, y - radius, radius * 2, radius * 2, QColor::fromRgba(qRgba(255, 255, 255, shadeBox)));
		painter->setPen(QColor::fromRgba(qRgba(0, 0, 0, shadeBox)));
		painter->drawRect(x - radius, y - radius, radius * 2, radius * 2);
		painter->drawLine(x - radius / 2, y, x + radius / 2, y);
	}
}

void PhylogenyTreeView::drawNodeLeaf(QPainter * painter, const PhylogenyNode * node, float highlight, float weightTop, float weightBottom) const
{
	PhylogenyNodeView & nodeView = nodeViews[node->getId()];
	
	int leaf = node->getLeafMin();
	int x = nodeView.x;
	int y = nodeView.y;
	
	float childSize = getTrackHeight(leaf + 1) - getTrackHeight(leaf);
	
	int shade;
	
	if
	(
		childSize >= 10 ||
		getTrackFocus() == leaf ||
		highlight
	)
	{
		shade = 255;
	}
	else if ( childSize < 1 )
	{
		shade = 0;
		//return;
	}
	else
	{
		shade = 256 - 256 * (10 - childSize) / 9;
	}
	
	int top = getTrackHeight(leaf);
	int bottom = getTrackHeight(leaf + 1);
	int height = bottom - top + 1;
	int radius = (getTrackHeight(leaf + 1) - getTrackHeight(leaf)) / 2;
	int right = getWidth() - 1;
	
	int bevelTop = y;
	int bevelBottom = y;
	
	int maxRadius = 9;
	
	if ( bevelTop - top > maxRadius )
	{
		bevelTop = top + maxRadius;
	}
	
	if ( bottom - bevelBottom > maxRadius )
	{
		bevelBottom = bottom - maxRadius;
	}
	
	if ( radius > maxRadius )
	{
		radius = maxRadius;
	}
	
	int bevelTopLeft = x + bevelTop - top;
	int bevelTopRight = getWidth() + top - bevelTop - 1;
	int bevelBottomLeft = x + bottom - bevelBottom;
	int bevelBottomRight = getWidth() + bevelBottom - bottom - 1;
	int rectLeft = bevelBottomLeft < bevelTopLeft ? bevelBottomLeft : bevelTopLeft;
	int rectWidth = bevelBottomLeft < bevelTopLeft ? bevelBottomRight - bevelBottomLeft : bevelTopRight - bevelTopLeft;
	//		shade = (shade + 230 * 1) / 2;
	//pen.setColor(qRgb(shade, shade, shade));
	QColor color;
	
	if ( nodeView.search )
	{
		if ( highlight )
		{
			color = qRgb(255, 255, 180);
		}
		else
		{
			color = qRgb(255, 255, 0);
		}
		
		shade = 255;
	}
	else if ( leaf == getTrackFocus() )
	{
		if ( highlight )
		{
			color = qRgb(230, 255, 255);
		}
		else
		{
			color = qRgb(200, 255, 255);
		}
		
		shade = 255;
	}
	else
	{
		int brightness;
		
		if ( highlight )
		{
			brightness = 235;
		}
		else
		{
			brightness = 220;
		}
		
		brightness = 220 + highlight * 20;
		
		color = QColor::fromHsl(leaf * 210 % 360, 50, brightness).rgb();
	}
	
	QPen pen;
	color.setAlpha(shade);
	QPainterPath path;
	path.moveTo(x + 1, bevelTop);
	path.lineTo(bevelTopLeft, top);
	path.lineTo(bevelBottomLeft, bottom);
	
	if ( bevelTop != bevelBottom )
	{
		path.lineTo(x, bevelBottom);
	}
	
	path.closeSubpath();
	painter->fillPath(path, color);
	painter->setPen(pen);
	painter->fillRect(rectLeft, top + 1, rectWidth, height - 2, color);
	
	QPainterPath path2;
	path2.moveTo(right, bevelTop);
	path2.lineTo(bevelTopRight, top);
	path2.lineTo(bevelBottomRight, bottom);
	
	if ( bevelTop != bevelBottom )
	{
		path2.lineTo(right, bevelBottom);
	}
	
	path2.closeSubpath();
	painter->fillPath(path2, color);
	
	QColor colorLine = qRgb(0, 0, 0);
	
	if ( leaf == getTrackFocus() )
	{
		colorLine = nodeView.search ? qRgb(0, 200, 200) : qRgb(0, 128, 128);
		weightTop = 1;
		weightBottom = 1;
	}
	
	drawLine(painter, x, bevelTop, bevelTopLeft, top, weightTop, colorLine);
	drawLine(painter, bevelTopLeft, top, bevelTopRight, top, weightTop, colorLine);
	
	drawLine(painter, bevelBottomLeft, bottom, bevelBottomRight, bottom, weightBottom, colorLine);
	drawLine(painter, x, bevelBottom, bevelBottomLeft, bottom, weightBottom, colorLine);
	
	if ( bevelTop != bevelBottom )
	{
		drawLine(painter, x, bevelTop, x, y, weightTop, colorLine);
		drawLine(painter, x, y, x, bevelBottom, weightBottom, colorLine);
	}
	
	float weightMin = weightTop < weightBottom ? weightTop : weightBottom;
	
	drawLine(painter, right, bevelTop, bevelTopRight, top, weightMin, colorLine);
	drawLine(painter, right, bevelBottom, bevelBottomRight, bottom, weightMin, colorLine);
	
	if ( bevelTop != bevelBottom )
	{
		drawLine(painter, right, bevelTop, right, bevelBottom, weightMin, colorLine);
	}
	
	int shadeText;
	if ( childSize >= 12 )
	{
		shadeText = 255;
	}
	else if ( childSize < 8 )
	{
		shadeText = 0;
	}
	else
	{
		shadeText = 256 - 256 * (12 - childSize) / 4;
	}
	pen.setColor(QColor::fromRgba(qRgba(0, 0, 0, shadeText)));
	painter->setPen(pen);
	//		painter->setFont(QFont(painter->font().family(), 12));//fontSize));
	//		painter->save();
	
	if ( shadeText > 0 )
	{
		float scale = (getTrackHeight(leaf + 1) - getTrackHeight(leaf)) * .8 / fontHeight;
		if ( scale > .5 )
		{
			scale = .5;
		}
		
		int textWidth = rectWidth + bevelBottom - bevelTop;
		
		painter->setOpacity((float)shadeText / 256);
		//		painter->scale(scale, scale);
		//		painter->drawText(QRect((x + radius + 1) / scale, top / scale, (getWidth() - (x + radius + 2) - 3) / scale, (height) / scale), Qt::AlignLeft | Qt::AlignVCenter, (*names)[(*order)[y]]);
		QImage * nameBuffer = nameBuffers[getIdByTrack(leaf)];
		/*painter->drawImage
		(
		 QRect(x + radius, (getTrackHeight(leaf + 1) + getTrackHeight(leaf)) / 2 + .5 - fontHeight * scale / 2, textWidth, fontHeight * scale), nameBuffer->scaled(nameBuffer->width() * scale, fontHeight * scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation), QRect(0, 0, textWidth, fontHeight * scale));*/
		painter->drawImage
		(
		 QRect(x + radius, (getTrackHeight(leaf + 1) + getTrackHeight(leaf)) / 2 + .5 - fontHeight * scale / 2, nameBuffer->width() * scale, nameBuffer->height() * scale), *nameBuffer, QRect(0, 0, nameBuffer->width(), nameBuffer->height()));
		//		painter->drawImage(x + yNode - top, top, *nameBuffers[y]);
		painter->setOpacity(1);
		//		painter->restore();
	}
}

float PhylogenyTreeView::maxVisibleDepth(const PhylogenyNode *node, float leafSize) const
{
	float max = 0;
	
	if ( nodeIsVisible(node, leafSize) )
	{
		max = node->getDepth();
		
		for ( int i = 0; i < node->getChildrenCount(); i++ )
		{
			float maxChild = maxVisibleDepth(node->getChild(i), leafSize);
			
			if ( maxChild > max )
			{
				max = maxChild;
			}
		}
	}
	
	return max;
}

void PhylogenyTreeView::updateNodeViews(const PhylogenyNode *node)
{
	PhylogenyNodeView & nodeView = nodeViews[node->getId()];
	
	nodeView.x = node->getDepth() * xFactor + xOffset;
	
	
	int childMinY;
	int childMaxY;
	
	for( int i = 0; i < node->getChildrenCount(); i++ )
	{
		updateNodeViews(node->getChild(i));
		
		int childY = nodeViews[node->getChild(i)->getId()].y;
		
		if( i == 0 )
		{
			childMinY = childY;
		}
		
		if( i == node->getChildrenCount() - 1 )
		{
			childMaxY = childY;
		}
	}
	
	if( node->getChildrenCount())
	{
		nodeView.y = (childMinY + childMaxY) / 2;
	}
	else
	{
		nodeView.y = (getTrackHeight(node->getLeafMin()) + getTrackHeight(node->getLeafMax() + 1)) / 2;
	}
}

float PhylogenyTreeView::zoomLerp(float start, float end)
{
	return start + getZoomProgress() * (end - start);
}
