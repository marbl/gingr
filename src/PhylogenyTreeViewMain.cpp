//
//  PhylogenyTreeViewMain.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/2/13.
//
//

#include "PhylogenyTreeViewMain.h"
#include <QMouseEvent>
#include <QToolTip>

void PhylogenyTreeViewMain::setTrackFocus(int track)
{
	PhylogenyTreeView::setTrackFocus(track);
	setWindow(focusNode);
}

void PhylogenyTreeViewMain::search(const QString & string, bool matchCase)
{
	int results = 0;
	
	if ( string == "" )
	{
		for ( int i = 0; i < getTrackCount(); i++ )
		{
			nodeViews[phylogenyTree->getLeaf(i)->getId()].search = false;
		}
	}
	else
	{
		for ( int i = 0; i < getTrackCount(); i++ )
		{
			QString name = (*names)[getIdByTrack(i)];
			QString search = string;
			
			if ( ! matchCase )
			{
				name = name.toLower();
				search = search.toLower();
			}
			
			bool result = name.contains(search);
			nodeViews[phylogenyTree->getLeaf(i)->getId()].search = result;
			
			if ( result )
			{
				results++;
			}
		}
	}
	
	emit signalSearchResults(results);
	setBufferUpdateNeeded();
}

bool PhylogenyTreeViewMain::event(QEvent * event)
{
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		
		if ( highlightNode != 0 && highlightNode->getChildrenCount() == 0 )
		{
			QToolTip::showText(helpEvent->globalPos(), (*names)[getIdByTrack(highlightNode->getLeafMin())]);
		}
		else
		{
			QToolTip::hideText();
			event->ignore();
		}
		
		return true;
	}
	
	return PhylogenyTreeView::event(event);
	
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

void PhylogenyTreeViewMain::mousePressEvent(QMouseEvent * event)
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
	else if ( highlightNode )
	{
		zoom(highlightNode);
	}
}

bool PhylogenyTreeViewMain::nodeIsVisible(const PhylogenyNode *node, float leafSize) const
{
	return getTrackFocus() != -1 || leafSize * node->getLeafCount() >= 5;
}

void PhylogenyTreeViewMain::resizeEvent(QResizeEvent *event)
{
	PhylogenyTreeView::resizeEvent(event);
	
	if ( focusNode != 0 )
	{
		setWindow(focusNode);
	}
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
		zoom(highlightNode);
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
			
			if ( highlightNode->getChildrenCount() == 0 )
			{
				setToolTip((*names)[getTrackById(highlightNode->getLeafMin())]);
			}
			else
			{
				QToolTip::hideText();
//				setToolTip("");
			}
		}
		else
		{
			QToolTip::hideText();
//			setToolTip("asdfasd");
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

void PhylogenyTreeViewMain::zoom(const PhylogenyNode * node)
{
	zoomIn = true;
	focusNodeLast.push_back(focusNode);
	setFocusNode(node);
	setZoomProgress(0);
	emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
}