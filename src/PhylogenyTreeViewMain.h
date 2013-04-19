//
//  PhylogenyTreeViewMain.h
//  gavqt
//
//  Created by Brian Ondov on 4/2/13.
//
//

#ifndef __gavqt__PhylogenyTreeViewMain__
#define __gavqt__PhylogenyTreeViewMain__

#include "PhylogenyTreeView.h"

class PhylogenyTreeViewMain : public PhylogenyTreeView
{
	Q_OBJECT
	
public:
	
	void setTrackFocus(int track);
	
protected:
	
	float getHighlight(const PhylogenyNode * node, float highlight, bool drawHighlight) const;
	QColor highlightColor(float highlight) const;
	bool nodeIsVisible(const PhylogenyNode *node, float leafSize) const;
	void resizeEvent(QResizeEvent *event);
	void updateTrackCursor();
	void wheelEvent(QWheelEvent * event);
	
signals:
	
	void signalFocusNode(const PhylogenyNode * node, bool zoomIn) const;
	
private:
	
	void checkMouse();
	bool checkMouseNode(const PhylogenyNode * node);
	void setFocusNode(const PhylogenyNode * node);
	
	QVector<const PhylogenyNode *> focusNodeLast;
};

#endif /* defined(__gavqt__PhylogenyTreeViewMain__) */
