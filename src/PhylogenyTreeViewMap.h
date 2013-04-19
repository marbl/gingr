//
//  PhylogenyTreeViewMap.h
//  gavqt
//
//  Created by Brian Ondov on 4/2/13.
//
//

#ifndef __gavqt__PhylogenyTreeViewMap__
#define __gavqt__PhylogenyTreeViewMap__

#include "PhylogenyTreeView.h"

class PhylogenyTreeViewMap : public PhylogenyTreeView
{
	Q_OBJECT
	
public:
	
	PhylogenyTreeViewMap();
	
public slots:
	
	void setFocusNode(const PhylogenyNode * node, bool newZoomIn);
	
protected:
	
	float getHighlight(const PhylogenyNode * node, float highlight, bool) const;
	QColor highlightColor(float highlight) const;
	bool nodeIsVisible(const PhylogenyNode *node, float leafSize) const;
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	
private:
	
	const PhylogenyNode * focusNodeLast;
};

#endif /* defined(__gavqt__PhylogenyTreeViewMap__) */
