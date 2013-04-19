//
//  MainWindow.h
//  gavqt
//
//  Created by Brian Ondov on 8/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_MainWindow_h
#define gavqt_MainWindow_h

#include <QWidget>
#include <QDomDocument>
#include <QVector>
#include "PhylogenyTreeViewMain.h"
#include "PhylogenyTreeViewMap.h"
#include "NameListView.h"
#include "AlignmentView.h"
#include "AnnotationView.h"
#include "BlockViewMain.h"
#include "BlockViewMap.h"
#include <QDateTime>
#include "Timer.h"
#include "RulerView.h"
#include "BlockStatusBar.h"
#include "PhylogenyTreeStatusBar.h"
#include "LcbView.h"
#include "ReferenceView.h"
#include "FilterControl.h"

class MainWindow : public QWidget
{
	Q_OBJECT
	
public:
	
	MainWindow(int argc, char ** argv, QWidget * parent = 0);
	~MainWindow();
	
public slots:
	
	void closeSnps();
	void toggleSnps(bool checked);
	void setNode(const PhylogenyNode * node);
	void setPosition(int gapped, int ungapped, int offset);
	void setTrackFocus(int track);
	void setTrackHover(int track, int trackEnd);
	void setTrackListViewFocus(TrackListView * view);
	void setTrackZoom(int start, int end);
	void setWindow(int start, int end);
	void unsetTrackListViewFocus(TrackListView * view);
	void update();
	
protected:
	
	void resizeEvent(QResizeEvent * event);
	
private:
	
	void connectTrackListView(TrackListView * view);
	bool loadXml(const QString & fileName);
	bool loadDomNames(const QDomElement * elementNames);
	void updateTrackHeights(bool setTargets = false);
	
	QVector<QString> names;
	QVector<QString> labels;
	PhylogenyTree tree;
	Alignment alignment;
	TrackListView * trackListViewFocus;
	PhylogenyTreeViewMain * treeViewMain;
	PhylogenyTreeViewMap * treeViewMap;
	NameListView * nameListView;
	AlignmentView * alignmentView;
	AlignmentView * alignmentView2;
	AlignmentView * alignmentView3;
	AnnotationView * annotationView;
	BlockViewMain * blockViewMain;
	BlockViewMap * blockViewMap;
	RulerView * rulerView;
	BlockStatusBar * blockStatus;
	PhylogenyTreeStatusBar * treeStatus;
	LcbView * lcbView;
	ReferenceView * referenceView;
	FilterControl * filterControl;
	QAction * actionSnps;
	
	QVector<int> leafIds;
	float * trackHeights;
	float * trackHeightsOverview;
	int trackCount;
	int cursorY;
	int trackFocus;
	int trackFocusLast;
	int trackHover;
	int trackHoverEnd;
	int trackZoomStart;
	int trackZoomEnd;
	int trackZoomStartLast;
	int trackZoomEndLast;
	int trackZoomScale;
	int trackZoomScaleLast;
	Tween tweenYFactor;
	Tween tweenYOffset;
	Timer timerFocus;
	Timer timerAlignment;
	Timer timerTrackZoom;
};

#endif
