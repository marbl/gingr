//
//  PhylogenyTreeViewMap.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/2/13.
//
//

#include "PhylogenyTreeViewMap.h"

PhylogenyTreeViewMap::PhylogenyTreeViewMap()
: PhylogenyTreeView()
{
	focusNodeLast = 0;
}

void PhylogenyTreeViewMap::setFocusNode(const PhylogenyNode *node, bool newZoomIn)
{
	focusNodeLast = focusNode;
	focusNode = node;
	zoomIn = newZoomIn;
}

float PhylogenyTreeViewMap::getHighlight(const PhylogenyNode *node, float highlight, bool) const
{
	if ( node == focusNode )
	{
		if ( getZoomProgress() < 1 && ! zoomIn )
		{
			highlight = getZoomProgress();
		}
		else
		{
			highlight = 1;
		}
	}
	else if ( node == focusNodeLast && getZoomProgress() < 1 )
	{
		if ( zoomIn )
		{
			highlight = 1 - getZoomProgress();
		}
		else
		{
			highlight = 1;
		}
	}
	
	return highlight;
}

QColor PhylogenyTreeViewMap::highlightColor(float highlight) const
{
	int shade = 235 + highlight * 20;
	int shade2 = 235 - highlight * 20;
	return qRgb(shade2, shade, shade);
}

bool PhylogenyTreeViewMap::nodeIsVisible(const PhylogenyNode *node, float leafSize) const
{
//	return leafSize * node->getLeafCount() >= 5;
	return node->getChildrenCount() > 0 || leafSize >= 5;
}

void PhylogenyTreeViewMap::paintEvent(QPaintEvent *event)
{
	PhylogenyTreeView::paintEvent(event);
}

void PhylogenyTreeViewMap::resizeEvent(QResizeEvent *event)
{
	PhylogenyTreeView::resizeEvent(event);
	setWindow(phylogenyTree->getRoot());
	setWindow(phylogenyTree->getRoot());
}