//
//  MainWindow.h
//  gavqt
//
//  Created by Brian Ondov on 8/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_MainWindow_h
#define gavqt_MainWindow_h

#include <QMainWindow>
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
#include "SearchControl.h"
#include "SnpBuffer.h"
#include "HarvestIO.h"
#include "SnapshotWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	
	MainWindow(int argc, char ** argv, QWidget * parent = 0);
	~MainWindow();
	
public slots:
	
	void closeSnps();
	void closeSearch();
	void menuActionHelp();
	void menuImportAlignment();
	void menuImportAnnotations();
	void menuImportTree();
	void menuOpen();
	void menuSnapshot();
	void toggleSnps(bool checked);
	void toggleSearch(bool checked);
	void toggleRightAlignNodes(bool checked);
	void toggleRightAlignText(bool checked);
	void toggleLightColors(bool checked);
	void saveSnapshot(const QString & fileName, bool tree, bool alignment);
	void setNode(const PhylogenyNode * node);
	void setPosition(int gapped);
	void toggleSynteny();
	void setTrackFocus(int track);
	void setTrackHover(int track, int trackEnd);
	void setTrackListViewFocus(TrackListView * view);
	void setTrackZoom(int start, int end);
	void setWindow(int start, int end);
	void setWindowTarget(int start, int end);
	void unsetTrackListViewFocus(TrackListView * view);
	void update();
	void updateSnpsFinishedMain();
	void updateSnpsMain();
	void updateSnpsMap();
	void zoomFromMouseWheel(int delta);
	
protected:
	
	void resizeEvent(QResizeEvent * event);
	
private:
	
	void connectTrackListView(TrackListView * view);
	void initialize();
	void initializeAlignment();
	void initializeTree();
	void loadAlignment(const QString & fileName);
	void loadAlignmentBackground(const QString & fileName);
	void loadAnnotations(const QString & fileName);
	void loadTree(const QString & fileName);
	bool loadPb(const QString & fileName);
	void loadPbBackground(const QString & fileName);
	bool loadPbNames(const Harvest::TrackList & data);
	bool loadXml(const QString & fileName);
	bool loadDomNames(const QDomElement * elementNames);
	void updateTrackHeights(bool setTargets = false);
	void updateTrackHeightsOverview();
	
	QVector<QString> names;
	QVector<QString> labels;
	PhylogenyTree * phylogenyTree;
	Alignment alignment;
	SnpBuffer snpBufferMain;
	SnpBuffer snpBufferMap;
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
	SearchControl * searchControl;
	SnapshotWindow * snapshotWindow;
	QWidget * help;
	QAction * actionSnps;
	QAction * actionSearch;
	QAction * actionToggleSynteny;
	float * leafDists;
	
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
	int posStart;
	int posEnd;
	int posCursor;
	bool synteny;
	bool lightColors;
	Tween tweenYFactor;
	Tween tweenYOffset;
	Tween tweenWindowStart;
	Tween tweenWindowEnd;
	int windowTargetStart;
	int windowTargetEnd;
	Timer timerFocus;
	Timer timerWindow;
	float zoom;
	
	HarvestIO hio;
};

#endif
