//
//  NameListView.h
//  gavqt
//
//  Created by Brian Ondov on 8/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_NameListView_h
#define gavqt_NameListView_h

#include "DrawingArea.h"
#include "PhylogenyTree.h"
#include "Track.h"
#include <QPainter>

class NameListView : public DrawingArea
{
	Q_OBJECT
	
public:
	
	NameListView();
	void setHeights(const float * newHeights);
	void setNames(QVector<QString> * newNames);
	void update();
	void setOrder(const QVector<unsigned int> * ids);
	void setHighlightTrack(int track);
	QSize sizeHint() const;
	
signals:
	
	void highlightTrackChanged(int track);
	
protected:
	
	void leaveEvent(QEvent *);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent * event);
	
private:
	
	QVector<QString> * names;
	const QVector<unsigned int> * order;
	const float * heights;
	int highlight;
	bool inDialog;
	int userWidth;
};

inline void NameListView::setHeights(const float *newHeights) {heights = newHeights;}
inline void NameListView::setNames(QVector<QString> *newNames) {names = newNames;}
inline void NameListView::setOrder(const QVector<unsigned int> *ids)
{
	order = ids;
}


#endif
