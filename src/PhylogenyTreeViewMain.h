// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__PhylogenyTreeViewMain__
#define __gavqt__PhylogenyTreeViewMain__

#include "PhylogenyTreeView.h"

class PhylogenyTreeViewMain : public PhylogenyTreeView
{
	Q_OBJECT
	
public:
	
	PhylogenyTreeViewMain();
	
	void clear();
	void setTrackFocus(int track);
	void setZoomProgress(float progress);
	
public slots:
	
	void reroot();
	void search(const QString & string, bool matchCase);
	
protected:
	
	bool event(QEvent * event);
	float getHighlight(const PhylogenyTreeNode * node, float highlight, bool drawHighlight) const;
	QColor highlightColor(float highlight) const;
	void leaveEvent(QEvent * event);
	void mousePressEvent(QMouseEvent * event);
	bool nodeIsVisible(const PhylogenyTreeNode *node, float leafSize) const;
	void resizeEvent(QResizeEvent *event);
	void updateTrackCursor();
	void wheelEvent(QWheelEvent * event);
	
signals:
	
	void signalContextMenu(bool inContextMenu);
	void signalFocusNode(const PhylogenyTreeNode * node, bool zoomIn) const;
	void signalReroot(const PhylogenyTreeNode * rootNew) const;
	void signalSearchResults(int results) const;
	
private:
	
	void checkMouse();
	bool checkMouseNode(const PhylogenyTreeNode * node);
	void setFocusNode(const PhylogenyTreeNode * node);
	void zoom(const PhylogenyTreeNode * node);
	
	QVector<const PhylogenyTreeNode *> focusNodeLast;
	bool contextMenu;
};

#endif /* defined(__gavqt__PhylogenyTreeViewMain__) */
