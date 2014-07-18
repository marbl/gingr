// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__PhylogenyTreeViewMap__
#define __gavqt__PhylogenyTreeViewMap__

#include "PhylogenyTreeView.h"

class PhylogenyTreeViewMap : public PhylogenyTreeView
{
	Q_OBJECT
	
public:
	
	PhylogenyTreeViewMap();
	
public slots:
	
	void setFocusNode(const PhylogenyTreeNode * node, bool newZoomIn);
	
protected:
	
	float getHighlight(const PhylogenyTreeNode * node, float highlight, bool) const;
	QColor highlightColor(float highlight) const;
	bool nodeIsVisible(const PhylogenyTreeNode *node, float leafSize) const;
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	
private:
	
	const PhylogenyTreeNode * focusNodeLast;
};

#endif /* defined(__gavqt__PhylogenyTreeViewMap__) */
