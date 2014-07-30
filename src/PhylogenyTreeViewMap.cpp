// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "PhylogenyTreeViewMap.h"

PhylogenyTreeViewMap::PhylogenyTreeViewMap()
: PhylogenyTreeView()
{
	focusNodeLast = 0;
}

void PhylogenyTreeViewMap::setFocusNode(const PhylogenyTreeNode *node, bool newZoomIn)
{
	focusNodeLast = focusNode;
	focusNode = node;
	zoomIn = newZoomIn;
	zoomOut = ! newZoomIn;
}

float PhylogenyTreeViewMap::getHighlight(const PhylogenyTreeNode *node, float highlight, bool) const
{
	if ( node == focusNode )
	{
		if ( getZoomProgress() < 1 && zoomOut )
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
		else if ( zoomOut )
		{
			highlight = 1;
		}
	}
	
	return highlight;
}

QColor PhylogenyTreeViewMap::highlightColor(float highlight) const
{
	int shade = 255;// + highlight * 20;
	int shade2 = 255 - highlight * 50;
	return qRgb(shade2, shade, shade);
}

bool PhylogenyTreeViewMap::nodeIsVisible(const PhylogenyTreeNode *node, float leafSize) const
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
	
	if ( phylogenyTree != 0 )
	{
		setWindow(phylogenyTree->getRoot());
		setWindow(phylogenyTree->getRoot());
	}
}