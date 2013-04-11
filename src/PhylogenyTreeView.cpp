//
//  PhylogenyTreeView.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "PhylogenyTreeView.h"
#include <stdlib.h>
#include <QMouseEvent>
#include <QtCore>

inline float absf(float x) {return x > 0 ? x : -x;}

PhylogenyTreeView::PhylogenyTreeView()
{
	phylogenyTree = 0;
	penDark.setColor(qRgb(0, 0, 0));
	penLight.setColor(qRgb(200, 200, 200));
	buffer = 0;
	nameBuffers = 0;
	nameBufferCount = 0;
	nodeViews = 0;
	highlightNode = 0;
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
//	focusNode = phylogenyTree->getRoot();
//	focusNodeLast = focusNode;
	setFocusNode(phylogenyTree->getRoot());
	setFocusNode(phylogenyTree->getRoot());
}

void PhylogenyTreeView::setTrackFocus(int track)
{
	TrackListView::setTrackFocus(track);
	focusNodeLast.push_back(focusNode);
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

void PhylogenyTreeView::updateTrackCursor()
{
//	TrackListView::mouseMoveEvent(event);
	checkMouse();
}

void PhylogenyTreeView::leaveEvent(QEvent *event)
{
	TrackListView::leaveEvent(event);
	
	highlightNode = 0;
}

void PhylogenyTreeView::mousePressEvent(QMouseEvent * event)
{
	if ( getTrackHover() == getTrackHoverEnd() )
	{
		if ( getTrackHover() == getTrackFocus() )
		{
			emit signalTrackFocusChange(-1);
		}
		else
		{
			emit signalTrackFocusChange(getTrackHover());
		}
	}
	else
	{
		emit signalTrackZoom(getTrackHover(), getTrackHoverEnd());
	}
}

void PhylogenyTreeView::paintEvent(QPaintEvent *event)
{
	TrackListView::paintEvent(event);
	
	if ( phylogenyTree == 0 )
	{
		return;
	}
	
	xFactor = zoomLerp(xFactorStart, xFactorEnd);
	xOffset = zoomLerp(xOffsetStart, xOffsetEnd);
	
	if ( redrawNeeded || bufferHighlight )
	{
		updateNodeViews(phylogenyTree->getRoot());
		QPainter painterBuffer(buffer);
		painterBuffer.fillRect(0, 0, buffer->width(), buffer->height(), qRgb(235, 235, 235));
		drawNode(&painterBuffer, phylogenyTree->getRoot(), redrawNeeded);
		bufferHighlight = redrawNeeded && highlightNode;
	}
	
	QPainter painter(this);
	painter.drawImage(frameWidth(), frameWidth(), *buffer);
	
	if ( ! redrawNeeded && highlightNode )
	{
		painter.save();
		painter.translate(1, 1);
		drawNode(&painter, highlightNode, true);
		painter.restore();
	}
	redrawNeeded = false;
}

void PhylogenyTreeView::resizeEvent(QResizeEvent * event)
{
	TrackListView::resizeEvent(event);
	radius = height() / 10;
	
	if ( buffer )
	{
		delete buffer;
	}
	
	buffer = new QImage(getWidth(), getHeight(), QImage::Format_RGB32);
	redrawNeeded = true;
}

void PhylogenyTreeView::wheelEvent(QWheelEvent * event)
{
	TrackListView::wheelEvent(event);
	
	if ( event->delta() < 0 )
	{
		if ( focusNode != phylogenyTree->getRoot() && getZoomProgress() == 1 )
		{
			setFocusNode(focusNodeLast.back());
			focusNodeLast.pop_back();
			zoomIn = false;
			emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
		}
	}
	else if ( highlightNode )
	{
		focusNodeLast.push_back(focusNode);
		setFocusNode(highlightNode);
		zoomIn = true;
		emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
	}
}

void PhylogenyTreeView::checkMouse()
{
	if ( getZoomProgress() < 1 )
	{
		highlightNode = 0;
		return;
	}
	
	const PhylogenyNode * highlightNodeLast = highlightNode;
	highlightNode = 0;
	checkMouseNode(focusNode);
	
	if ( highlightNode != highlightNodeLast )
	{
		if ( highlightNode )
		{
			signalTrackHoverChange(highlightNode->getLeafMin(), highlightNode->getLeafMax());
		}
		else
		{
			signalTrackHoverChange(-1, -1);
		}
	}
}

bool PhylogenyTreeView::checkMouseNode(const PhylogenyNode *node)
{
	PhylogenyNodeView & nodeView = nodeViews[node->getId()];
	
	if ( getCursorY() < getTrackHeight(node->getLeafMin()) || getCursorY() > getTrackHeight(node->getLeafMax() + 1) || getCursorX() < nodeView.x )
	{
		return false;
	}
	
	float childSize = getTrackHeight(node->getLeafMax() + 1) - getTrackHeight(node->getLeafMin());
	
	if ( childSize < 10 )
	{
		return false;
	}
	
	for ( int i = 0; i < node->getChildrenCount(); i++ )
	{
		if ( checkMouseNode(node->getChild(i)) )
		{
			return true;
		}
	}
	
	int childMinY;
	int childMaxY;
	
	if ( node->getChildrenCount() )
	{
		childMinY = nodeViews[node->getChild(0)->getId()].y;
		childMaxY = nodeViews[node->getChild(node->getChildrenCount() - 1)->getId()].y;
	}
	else
	{
		childMinY = getTrackHeight(node->getLeafMin());
		childMaxY = getTrackHeight(node->getLeafMax() + 1);
	}
	
	if ( getCursorY() >= childMinY && getCursorY() <= childMaxY && node != focusNode )
	{
		highlightNode = node;
		emit signalNodeHover(node);
		return true;
	}
	else
	{
		return false;
	}
}

void PhylogenyTreeView::drawLine(QPainter * painter, int x1, int y1, int x2, int y2, float weight, QColor color) const
{
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
		
		pen.setColor(QColor::fromRgba(qRgba(color.red(), color.green(), color.blue(), 255 * ((weight - .3) / 1.5))));
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
		painter->setPen(QColor::fromRgba(qRgba(color.red(), color.green(), color.blue(), 255 * (weight / .3))));
		painter->drawLine(x1, y1, x2, y2);
	}
}


void PhylogenyTreeView::drawNode(QPainter * painter, const PhylogenyNode *node, bool drawHighlight, float highlight, float weightTop, float weightBottom) const
{
	PhylogenyNodeView & nodeView = nodeViews[node->getId()];
	
	float childSize = getTrackHeight(node->getLeafMax() + 1) - getTrackHeight(node->getLeafMin());
	int x = nodeView.x;
	
	if ( drawHighlight )
	{
		if ( node == highlightNode )
		{
			highlight = 1;
		}
		else if ( node == focusNode && getZoomProgress() < 1 && zoomIn )
		{
			highlight = 1 - getZoomProgress();
		}
	}
	
	float weight;
	float maxHeight = 150;
	float minHeight = 4;
	
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
		
		int shade = 235 + highlight * 20;
		
//		painter->fillRect(x + 1, childMinY, getWidth() - x - 1, childMaxY - childMinY, qRgb(245, bootShade, bootShade));
		
		if ( highlight )
		{
//			painter->setOpacity(1 - getZoomProgress());
			painter->fillRect(x + 1, childMinY, getWidth() - x - 1, childMaxY - childMinY, qRgb(shade, shade, shade));
//			painter->setOpacity(1);
		}
		
		pen.setColor(QColor::fromRgba(qRgba(bootShade, 0, 0, 255 * maxf(weight, weightTop))));
		painter->setPen(pen);
//		painter->drawLine(x, childMinY, x, nodeView.y);
		drawLine(painter, x, childMinY, x, nodeView.y, maxf(weight, weightTop), qRgb(bootShade, 0, 0));
		
		pen.setColor(QColor::fromRgba(qRgba(bootShade, 0, 0, 255 * maxf(weight, weightBottom))));
		painter->setPen(pen);
//		painter->drawLine(x, nodeView.y, x, childMaxY);
		drawLine(painter, x, childMaxY, x, nodeView.y, maxf(weight, weightBottom), qRgb(bootShade, 0, 0));
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
	
	float dist = qSqrt(qPow(getCursorX() - x, 2) + qPow(getCursorY() - y, 2));
	
	int radius = 6;
	if ( ! highlightNode && node->getChildrenCount() && getCursorX() >= x - radius && getCursorX() <= x + radius && getCursorY() >= y - radius && getCursorY() <= y + radius )//dist < 20)
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
		return;
	}
	else
	{
		shade = 256 - 256 * (10 - childSize) / 9;
	}
	
	int top = getTrackHeight(leaf);
	int bottom = getTrackHeight(leaf + 1);
	int height = bottom - top + 1;
	int radius = (getTrackHeight(leaf + 1) - getTrackHeight(leaf)) / 2;
	
	//		shade = (shade + 230 * 1) / 2;
	//pen.setColor(qRgb(shade, shade, shade));
	QColor color;
	
	if ( leaf == getTrackFocus() )
	{
		color = qRgb(255, 255, 255);
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
		
		brightness = 220 + highlight * 15;
		
		color = QColor::fromHsl(leaf * 210 % 360, 50, brightness).rgb();
	}
	
	int x = nodeView.x;
	QPen pen;
	color.setAlpha(shade);
	QPainterPath path;
	path.moveTo(x + 1, y);
	path.lineTo(x + y - top + 1, top);
	path.lineTo(x + bottom - y, bottom);
	path.closeSubpath();
	painter->fillPath(path, color);
	painter->setPen(pen);
	//		painter->drawLine(x + radius, yNode, width() - frameWidth() * 2, yNode);
	painter->fillRect(x + radius, top + 1, getWidth() - (x + radius * 2), height - 2, color);
	QPainterPath path2;
	path2.moveTo(getWidth() - 1, y);
	path2.lineTo(getWidth() + top - y - 1, top);
	path2.lineTo(getWidth() + y - bottom - 1, bottom);
	path2.closeSubpath();
	painter->fillPath(path2, color);
	pen.setWidth(1);
	//		pen.setCapStyle(Qt::SquareCap);
	pen.setColor(QColor::fromRgba(qRgba(0, 0, 0, shade)));
	painter->setPen(pen);
	//		painter->drawArc(x, yLeaves[y] + frameWidth(), height, (yLeaves[y + 1] - yLeaves[y] + 1), 90 * 16, 180 * 16);
	
	QColor colorLine = qRgb(0, 0, 0);
	
	pen.setColor(QColor::fromRgba(qRgba(100, 100, 100, 255 * weightTop)));
	painter->setPen(pen);
//	painter->drawLine(x, y, x + y - top, top); // top left
//	painter->drawLine(x + y - top + 1, top, getWidth() - radius - 1, top); // top
	drawLine(painter, x, y, x + y - top, top, weightTop, colorLine); // top left
	drawLine(painter, x + y - top + 1, top, getWidth() - radius - 1, top, weightTop, colorLine); // top
	
	pen.setColor(QColor::fromRgba(qRgba(100, 100, 100, 255 * weightBottom)));
	painter->setPen(pen);
//	painter->drawLine(x + bottom - y + 1, bottom, getWidth() - radius - 1, bottom);
//	painter->drawLine(x, y, x + bottom - y, bottom); // bottom left
	drawLine(painter, x + bottom - y + 1, bottom, getWidth() - radius - 1, bottom, weightBottom, colorLine);
	drawLine(painter, x, y, x + bottom - y, bottom, weightBottom, colorLine); // bottom left
	
	float weightMin = weightTop < weightBottom ? weightTop : weightBottom;
	
	pen.setColor(QColor::fromRgba(qRgba(0, 0, 0, 255 * (weightTop < weightBottom ? weightTop : weightBottom))));
	painter->setPen(pen);
//	painter->drawLine(getWidth() - 1, y, getWidth() + top - y - 1, top); // top r
//	painter->drawLine(getWidth() - 1, y, getWidth() + y - bottom - 1, bottom); //br
	drawLine(painter, getWidth() - 1, y, getWidth() + top - y - 1, top, weightMin, colorLine); // top r
	drawLine(painter, getWidth() - 1, y, getWidth() + y - bottom - 1, bottom, weightMin, colorLine); //br
	
	int shadeText;
	if ( childSize >= 10 )
	{
		shadeText = 255;
	}
	else if ( childSize < 4 )
	{
		shadeText = 0;
	}
	else
	{
		shadeText = 256 - 256 * (10 - childSize) / 6;
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
		painter->setOpacity((float)shadeText / 256);
		//		painter->scale(scale, scale);
		//		painter->drawText(QRect((x + radius + 1) / scale, top / scale, (getWidth() - (x + radius + 2) - 3) / scale, (height) / scale), Qt::AlignLeft | Qt::AlignVCenter, (*names)[(*order)[y]]);
		QImage * nameBuffer = nameBuffers[(*order)[leaf]];
		painter->drawImage(QRect(x + radius, top + ((float)radius + .5 - fontHeight * scale / 2) + 1, getWidth() - (x + radius + 2) - 3, fontHeight * scale), nameBuffer->scaled(nameBuffer->width() * scale, fontHeight * scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation), QRect(0, 0, getWidth() - (x + radius + 2) - 3, fontHeight * scale));
		//		painter->drawImage(x + yNode - top, top, *nameBuffers[y]);
		painter->setOpacity(1);
		//		painter->restore();
	}
}

float PhylogenyTreeView::maxVisibleDepth(const PhylogenyNode *node, float leafSize) const
{
	float max = 0;
	
	if ( leafSize * node->getLeafCount() >= 5 )
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

void PhylogenyTreeView::setFocusNode(const PhylogenyNode * node)
{
	focusNode = node;
	highlightNode = 0;
	
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
	
	float leafSize = (float)getHeight() / focusNode->getLeafCount();
	xFactorEnd = (getWidth() - (leafSize >= 5 ? nameWidth : 0)) / (maxVisibleDepth(focusNode, leafSize) - focusNode->getDepth());
	xOffsetEnd = -xFactorEnd * focusNode->getDepth();
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
