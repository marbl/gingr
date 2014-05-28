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
	
	PhylogenyTreeViewMain();
	
	void setTrackFocus(int track);
	
public slots:
	
	void reroot();
	void search(const QString & string, bool matchCase);
	
protected:
	
	bool event(QEvent * event);
	float getHighlight(const PhylogenyNode * node, float highlight, bool drawHighlight) const;
	QColor highlightColor(float highlight) const;
	void leaveEvent(QEvent * event);
	void mousePressEvent(QMouseEvent * event);
	bool nodeIsVisible(const PhylogenyNode *node, float leafSize) const;
	void resizeEvent(QResizeEvent *event);
	void updateTrackCursor();
	void wheelEvent(QWheelEvent * event);
	
signals:
	
	void signalContextMenu(bool inContextMenu);
	void signalFocusNode(const PhylogenyNode * node, bool zoomIn) const;
	void signalReroot(const PhylogenyNode * rootNew) const;
	void signalSearchResults(int results) const;
	
private:
	
	void checkMouse();
	bool checkMouseNode(const PhylogenyNode * node);
	void setFocusNode(const PhylogenyNode * node);
	void zoom(const PhylogenyNode * node);
	
	QVector<const PhylogenyNode *> focusNodeLast;
	bool contextMenu;
};

#endif /* defined(__gavqt__PhylogenyTreeViewMain__) */
