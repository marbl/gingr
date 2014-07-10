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
#include <QSettings>
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
#include "harvest/HarvestIO.h"
#include "SnapshotWindow.h"
#include <vector>

enum AlignmentType
{
	MFA,
	VCF,
	XMFA,
	XMFA_REF,
};

enum ExportType
{
	ALIGNMENT_XMFA,
	TREE,
	VARIANT_MFA,
	VARIANT_VCF,
};

static const QString DEFAULT_DIR_KEY("default_dir");

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	
	MainWindow(int argc, char ** argv, QWidget * parent = 0);
	~MainWindow();
	
public slots:
	
	void closeSnps();
	void closeSearch();
	bool documentChanged();
	void menuActionHelp();
	void menuImportAlignmentMfa();
	void menuImportAlignmentVcfFasta();
	void menuImportAlignmentXmfa();
	void menuImportAlignmentXmfaFasta();
	void menuImportAnnotations();
	void menuImportTree();
	void menuExportAlignmentXmfa();
	void menuExportTree();
	void menuExportVariantsMfa();
	void menuExportVariantsVcf();
	void menuNew();
	void menuOpen();
	bool menuSave();
	bool menuSaveAs();
	void menuSnapshot();
	bool promptSave();
	void rerootTree(const PhylogenyTreeNode * rootNew = 0);
	void rerootTreeMidpoint();
	void setInContextMenu(bool inContextMenuNew);
	void toggleShowGaps(bool checked);
	void toggleShowInsertions(bool checked);
	void toggleShowDeletions(bool checked);
	void toggleSnps(bool checked);
	void toggleSearch(bool checked);
	void toggleRightAlignNodes(bool checked);
	void toggleRightAlignText(bool checked);
	void toggleLightColors(bool checked);
	void saveSnapshot(const QString & fileName, bool tree, bool alignment);
	void setDocumentChanged();
	void setDocumentLoaded();
	void setDocumentUnchanged();
	void setDocumentUnloaded();
	void setNode(const PhylogenyTreeNode * node);
	void setPosition(int gapped);
	void toggleSynteny();
	void setTrackFocus(int track);
	void setTrackHover(int track, int trackEnd);
	void setTrackListViewFocus(TrackListView * view);
	void setTrackReference(int trackReferenceNew);
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
	
	void clear();
	void connectTrackListView(TrackListView * view);
	void exportFile(const QString & fileName, ExportType type);
	void exportFileBackground(const QString & fileName, ExportType type);
	const QString getDefaultDirectory();
	void initialize();
	void initializeAlignment();
	void initializeLayout();
	void initializeTree();
	void loadAlignment(const QString & fileName, const QString &fileNameRef, AlignmentType type);
	void loadAlignmentBackground(const QString & fileName, const QString &fileNameRef, AlignmentType type);
	void loadAnnotations(const QString & fileName);
	void loadTree(const QString & fileName);
	bool loadHarvest(const QString & fileName);
	void loadHarvestBackground(const QString & fileName);
	bool loadNames(const TrackList & data);
	bool loadDomNames(const QDomElement * elementNames);
	void setDefaultDirectoryFromFile(const QString & file);
	void updateTrackHeights(bool setTargets = false);
	void updateTrackHeightsOverview();
	void writeHarvest();
	void writeHarvestBackground();
	
	QVector<QString> names;
	QVector<QString> labels;
	//PhylogenyTree * phylogenyTree;
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
	QMenu * menuHelp;
	QAction * actionExportAlignmentXmfa;
	QAction * actionExportImage;
	QAction * actionExportTree;
	QAction * actionExportVariantsMfa;
	QAction * actionExportVariantsVcf;
	QAction * actionImportAnnotations;
	QAction * actionSave;
	QAction * actionSaveAs;
	QAction * actionSnps;
	QAction * actionSearch;
	QAction * actionToggleShowInsertions;
	QAction * actionToggleShowDeletions;
	QAction * actionToggleSynteny;
	float * leafDists;
	QString harvestFileCurrent;
	
	std::vector<int> leafIds;
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
	int showGaps;
	bool inContextMenu;
	bool loading;
	
	HarvestIO hio;
};

#endif
