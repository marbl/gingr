// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "PhylogenyTreeViewMain.h"
#include <QMouseEvent>
#include <QToolTip>
#include <QMenu>

PhylogenyTreeViewMain::PhylogenyTreeViewMain()
{
	contextMenu = false;
}

void PhylogenyTreeViewMain::clear()
{
	PhylogenyTreeView::clear();
	
	focusNodeLast.clear();
}

void PhylogenyTreeViewMain::setTrackFocus(int track)
{
	PhylogenyTreeView::setTrackFocus(track);
	
	if ( phylogenyTree )
	{
		setWindow(focusNode);
	}
}

void PhylogenyTreeViewMain::reroot()
{
	emit signalReroot(highlightNode);
}

void PhylogenyTreeViewMain::exportDifferential()
{
	emit signalExportDifferential(highlightNode);
}

void PhylogenyTreeViewMain::exportSignature()
{
	emit signalExportSignature(highlightNode);
}

void PhylogenyTreeViewMain::search(const QString & string, bool matchCase)
{
	int results = 0;
	
	if ( string == "" )
	{
		for ( int i = 0; i < getTrackCount(); i++ )
		{
			int index = phylogenyTree ? phylogenyTree->getLeaf(i)->getId() : i;
			nodeViews[index].search = false;
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
			int index = phylogenyTree ? phylogenyTree->getLeaf(i)->getId() : i;
			nodeViews[index].search = result;
			
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

float PhylogenyTreeViewMain::getHighlight(const PhylogenyTreeNode *node, float highlight, bool drawHighlight) const
{
	if ( drawHighlight )
	{
		if ( node == highlightNode )
		{
			highlight = 1;
		}
		else if ( node == focusNode && getZoomProgress() < 1 && zoomIn )
		{
			highlight = 1. - getZoomProgress();
		}
	}
	
	return highlight;
}

QColor PhylogenyTreeViewMain::highlightColor(float highlight) const
{
//	return Qt::yellow; // TEMP
	int shade = 255 - highlight * 20;
//	printf("highlight: %f\tshade: %d\n", highlight, shade);
	return qRgb(shade, 255, 255);
}

void PhylogenyTreeViewMain::leaveEvent(QEvent * event)
{
	if ( ! contextMenu )
	{
		PhylogenyTreeView::leaveEvent(event);
	}
}

void PhylogenyTreeViewMain::mousePressEvent(QMouseEvent * event)
{
	if ( phylogenyTree == 0 )
	{
		if ( getTrackCount() && getTrackHover() != -1 )
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
		
		return;
	}
	
	rightAlignLast = rightAlign;
	
	
	if ( event->button() == Qt::RightButton )
	{
		if ( highlightNode && highlightNode != focusNode )
		{
			QMenu * menuContext = new QMenu(this);
			
			QAction * actionReroot = new QAction(tr("Set as outgroup"), this);
			connect(actionReroot, SIGNAL(triggered()), this, SLOT(reroot()));
			menuContext->addAction(actionReroot);
			
			if ( highlightNode->getChildrenCount() > 0 )
			{
				QAction * actionDiff = new QAction(tr("Export differential variants as VCF"), this);
				connect(actionDiff, SIGNAL(triggered()), this, SLOT(exportDifferential()));
				menuContext->addAction(actionDiff);
				
				QAction * actionSign = new QAction(tr("Export signature variants as VCF"), this);
				connect(actionSign, SIGNAL(triggered()), this, SLOT(exportSignature()));
				menuContext->addAction(actionSign);
			}
			
			contextMenu = true;
			emit signalContextMenu(true);
			menuContext->exec(event->globalPos());
			emit signalContextMenu(false);
			contextMenu = false;
			
			highlightNode = 0;
			emit signalTrackHoverChange(-1, -1);
//			setUpdateNeeded();
		}
		/*
		if ( highlightNode )
		{
			useGradient = ! useGradient;
		}
		else
		{
			leafLines = ! leafLines;
		}
		 */
	}
	else
	{
		/*
		if ( highlightNode )
		{
			showDots = ! showDots;
		}
		else
		{
			rightAlign = ! rightAlign;
		}
		*/
		
		if ( highlightNode == focusNode || highlightNode == 0 )
		{
			if ( focusNode != phylogenyTree->getRoot() && getZoomProgress() == 1 )
			{
				zoomIn = false;
				zoomOut = true;
				setFocusNode(focusNodeLast.back());
				setZoomProgress(0);
				focusNodeLast.pop_back();
				emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
				setCursor(Qt::ArrowCursor);
			}
		}
		else if ( getTrackHover() == getTrackHoverEnd() )
		{
			if ( getTrackHover() == getTrackFocus() )
			{
				emit signalTrackFocusChange(-1);
			}
			else
			{
				if ( false && event->button() == Qt::RightButton )
				{
					emit signalTrackFocusChange(getTrackFocus()); // HACK
				}
				else
				{
					emit signalTrackFocusChange(getTrackHover());
				}
			}
		}
		else if ( highlightNode )
		{
			zoom(highlightNode);
		}
	}
}

bool PhylogenyTreeViewMain::nodeIsVisible(const PhylogenyTreeNode *node, float leafSize) const
{
	return getTrackFocus() != -1 || leafSize * node->getLeafCount() >= 8;
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
	
	if ( getZoomProgress() < 1 || phylogenyTree == 0 )
	{
		return;
	}
	
	if ( event->delta() < -2 )
	{
		if ( focusNode != phylogenyTree->getRoot() && getZoomProgress() == 1 )
		{
			zoomIn = false;
			zoomOut = true;
			setFocusNode(focusNodeLast.back());
			setZoomProgress(0);
			focusNodeLast.pop_back();
			emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
		}
	}
	else if ( event->delta() > 2 && highlightNode && getTrackHover() != getTrackHoverEnd() )
	{
		zoom(highlightNode);
	}
}

void PhylogenyTreeViewMain::updateTrackCursor()
{
	if ( phylogenyTree )
	{
		if ( ! contextMenu )
		{
			checkMouse();
		}
	}
	else
	{
		PhylogenyTreeView::updateTrackCursor();
	}
}

void PhylogenyTreeViewMain::checkMouse()
{
	if ( getZoomProgress() < 1 || names == 0 || getCursorX() == -1 )
	{
		return;
	}
	
	const PhylogenyTreeNode * highlightNodeLast = highlightNode;
	highlightNode = 0;
	
	checkMouseNode(focusNode);
	
	if ( highlightNode || focusNode == phylogenyTree->getRoot() )
	{
		setCursor(Qt::ArrowCursor);
	}
	else
	{
		setCursor(Qt::UpArrowCursor);
	}
	
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

bool PhylogenyTreeViewMain::checkMouseNode(const PhylogenyTreeNode *node)
{
	PhylogenyTreeNodeView & nodeView = nodeViews[node->getId()];
	
	if ( getCursorY() < getTrackHeight(node->getLeafMin()) || getCursorY() > getTrackHeight(node->getLeafMax() + 1) || getCursorX() < nodeView.getX() )
	{
		return false;
	}
	
	float childSize = getTrackHeight(node->getLeafMax() + 1) - getTrackHeight(node->getLeafMin());
	
	if ( childSize < 10 )
	{
		//return false;
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
	
	if ( (childSize >= 10 || node->getChildrenCount() == 0) && getCursorY() >= childMinY && getCursorY() <= childMaxY && node != focusNode )
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

void PhylogenyTreeViewMain::setFocusNode(const PhylogenyTreeNode * node)
{
	focusNode = node;
	highlightNode = 0;
	setWindow(focusNode);
	groupNodes(focusNode);
	emit signalFocusNode(focusNode, zoomIn);
}

void PhylogenyTreeViewMain::zoom(const PhylogenyTreeNode * node)
{
	zoomIn = true;
	zoomOut = false;
	focusNodeLast.push_back(focusNode);
	setFocusNode(node);
	setZoomProgress(0);
	emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
}

void PhylogenyTreeViewMain::zoomTop()
{
	if ( focusNode == phylogenyTree->getRoot() )
	{
		return;
	}
	
	zoomIn = false;
	zoomOut = true;
	setFocusNode(phylogenyTree->getRoot());
	setZoomProgress(0);
	focusNodeLast.clear();
	emit signalTrackZoom(focusNode->getLeafMin(), focusNode->getLeafMax());
}