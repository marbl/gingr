//
//  PhylogenyTreeViewMain.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/2/13.
//
//

#include "PhylogenyTreeViewMain.h"
#include <QMouseEvent>

void PhylogenyTreeViewMain::setTrackFocus(int track)
{
	PhylogenyTreeView::setTrackFocus(track);
	setWindow(focusNode);
}

float PhylogenyTreeViewMain::getHighlight(const PhylogenyNode *node, float highlight, bool drawHighlight) const
{
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
	
	return highlight;
}

QColor PhylogenyTreeViewMain::highlightColor(float highlight) const
{
	int shade = 235 + highlight * 20;
	return qRgb(shade, shade, shade);
}

bool PhylogenyTreeViewMain::nodeIsVisible(const PhylogenyNode *node, float leafSize) const
{
	return getTrackFocus() != -1 || leafSize * node->getLeafCount() >= 5;
}

void PhylogenyTreeViewMain::resizeEvent(QResizeEvent *event)
{
	PhylogenyTreeView::resizeEvent(event);
	setWindow(focusNode);
}

void PhylogenyTreeViewMain::wheelEvent(QWheelEvent * event)
{
	TrackListView::wheelEvent(event);
	
	if ( getZoomProgress() < 1 )
	{
		return;
	}
	
	if ( event->delta() < 0 )
	{
		if ( focusNode != phylogenyTree->getRoot() && getZoomProgress() == 1 )
		{
			zoomIn = false;
			setFocusNode(focusNodeLast.back());
			setZoomProgress(0);
			focusNodeLast.pop_back();
			emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
		}
	}
	else if ( highlightNode )
	{
		zoomIn = true;
		focusNodeLast.push_back(focusNode);
		setFocusNode(highlightNode);
		setZoomProgress(0);
		emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
	}
}

void PhylogenyTreeViewMain::updateTrackCursor()
{
	//	TrackListView::mouseMoveEvent(event);
	checkMouse();
}

void PhylogenyTreeViewMain::checkMouse()
{
	const PhylogenyNode * highlightNodeLast = highlightNode;
	highlightNode = 0;
	
	if ( getZoomProgress() < 1 )
	{
		return;
	}
	
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

bool PhylogenyTreeViewMain::checkMouseNode(const PhylogenyNode *node)
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

void PhylogenyTreeViewMain::setFocusNode(const PhylogenyNode * node)
{
	focusNode = node;
	highlightNode = 0;
	setWindow(focusNode);
	emit signalFocusNode(focusNode, zoomIn);
}

