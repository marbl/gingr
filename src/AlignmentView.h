//
//  AlignmentView.h
//  gavqt
//
//  Created by Brian Ondov on 8/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_AlignmentView_h
#define gavqt_AlignmentView_h

#include "Alignment.h"
#include "TrackListView.h"
#include "TrackView.h"

class AlignmentView : public TrackListView
{
	Q_OBJECT
	
public:
	
	AlignmentView(int hueStart, int hueEnd);
	~AlignmentView();
	
	bool getTweenNeeded() const;
	void setAlignment(const Alignment * newAlignment);
	void update(float newProgress);
	
public slots:
	
	void setLcbHover(int lcb, float offset);
	
protected:
	
	void leaveEvent(QEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event);
	
signals:
	
	void signalLcbHoverChange(int lcb, float offset);
	
private:
	
	void deletePalette();
	void drawLcbs(QPainter * painter, int id1, int id2);
	float getRefPos(int lcb, float offset) const;
	void initializePalette(int hueStart, int hueEnd);
	void setColors();
	
	const Alignment * alignment;
	int secLast;
	int frames;
	QVector<QRgb> colors;
	QRgb ** palette;
	QVector<TrackView> trackViews;
	int cursorX;
	int cursorY;
	float cursorSize;
	bool highlight;
	bool highlightGradient;
	int highlightTrack;
	int highlightLcb;
	float highlightOffset;
	bool highlightRc;
	int markTrack;
	int markLcb;
	float markOffset;
	bool markRc;
	float progress;
	RegionVector refByLcb;
	unsigned int refSize;
	bool tweenNeeded;
};

inline bool AlignmentView::getTweenNeeded() const {return tweenNeeded;}

#endif
