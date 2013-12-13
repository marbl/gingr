//
//  MainWindow.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MainWindow.h"
#include <QtGui>
#include <time.h>
#include "LinkedSplitter.h"
#include "OptionButton.h"
#include <QMenuBar>
#include <fstream>
#include <QTextBrowser>

//#include <google/protobuf/io/coded_stream.h>
//#include <google/protobuf/io/zero_copy_stream_impl.h>
//#include <google/protobuf/io/gzip_stream.h>
//#include <fcntl.h>
#include <QFileInfo>

//using namespace google::protobuf::io;

MainWindow::MainWindow(int argc, char ** argv, QWidget * parent)
: QMainWindow(parent)
{
	trackFocus = -1;
	trackFocusLast = -1;
	trackListViewFocus = 0;
	trackCount = 0;
	synteny = false;
	trackHeights = 0;
	trackHeightsOverview = 0;
	lightColors = false;
	zoom = 1;
	help = 0;
	
	phylogenyTree = 0;
	
	setWindowTitle(tr("Gingr"));
	
	QWidget * widgetCentral = new QWidget(this);
	setCentralWidget(widgetCentral);
	
	//QMenuBar * menuBar = new QMenuBar(this);
	QMenu * menuFile = menuBar()->addMenu("File");
	QMenu * menuView = menuBar()->addMenu("View");
	QMenu * menuWindow = menuBar()->addMenu("Window");
	QMenu * menuHelp = menuBar()->addMenu("Help");
	
	QAction * actionOpen = new QAction(tr("&Open Harvest"), this);
	actionOpen->setShortcut(QKeySequence("Ctrl+O"));
	menuFile->addAction(actionOpen);
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(menuOpen()));
	
	QAction * actionSeparator1 = new QAction(this);
	actionSeparator1->setSeparator(true);
	menuFile->addAction(actionSeparator1);
	
	QAction * actionImportAlignment = new QAction(tr("Import alignment"), this);
	//menuFile->addAction(actionImportAlignment);
	connect(actionImportAlignment, SIGNAL(triggered()), this, SLOT(menuImportAlignment()));
	
	QAction * actionImportAnnotations = new QAction(tr("Import annotations"), this);
	//menuFile->addAction(actionImportAnnotations);
	connect(actionImportAnnotations, SIGNAL(triggered()), this, SLOT(menuImportAnnotations()));
	
	QAction * actionImportTree = new QAction(tr("Import tree"), this);
	//menuFile->addAction(actionImportTree);
	connect(actionImportTree, SIGNAL(triggered()), this, SLOT(menuImportTree()));
	
	QAction * actionSeparator2 = new QAction(this);
	actionSeparator2->setSeparator(true);
	menuFile->addAction(actionSeparator2);
	
	QAction * actionExportImage = new QAction(tr("Sna&pshot..."), this);
	actionExportImage->setShortcut(QKeySequence("Ctrl+P"));
	menuFile->addAction(actionExportImage);
	connect(actionExportImage, SIGNAL(triggered()), this, SLOT(menuSnapshot()));
	
	actionToggleSynteny = new QAction(tr("Synten&y"), this);
	actionToggleSynteny->setCheckable(true);
	actionToggleSynteny->setShortcut(QKeySequence("Ctrl+Y"));
	menuView->addAction(actionToggleSynteny);
	connect(actionToggleSynteny, SIGNAL(triggered()), this, SLOT(toggleSynteny()));
	
	QAction * actionSeparator3 = new QAction(this);
	actionSeparator3->setSeparator(true);
	menuView->addAction(actionSeparator3);
	
	QAction * actionRightAlignNodes = new QAction(tr("&Right-align clades"), this);
	actionRightAlignNodes->setCheckable(true);
	actionRightAlignNodes->setShortcut(QKeySequence("Ctrl+R"));
	menuView->addAction(actionRightAlignNodes);
	connect(actionRightAlignNodes, SIGNAL(toggled(bool)), this, SLOT(toggleRightAlignNodes(bool)));
	
	QAction * actionRightAlignText = new QAction(tr("&Right-align labels"), this);
	actionRightAlignText->setShortcut(QKeySequence("Ctrl+T"));
	actionRightAlignText->setCheckable(true);
	menuView->addAction(actionRightAlignText);
	connect(actionRightAlignText, SIGNAL(toggled(bool)), this, SLOT(toggleRightAlignText(bool)));
	
	QAction * actionToggleLightColors = new QAction(tr("&Light colors"), this);
	actionToggleLightColors->setShortcut(QKeySequence("Ctrl+L"));
	actionToggleLightColors->setCheckable(true);
	menuView->addAction(actionToggleLightColors);
	connect(actionToggleLightColors, SIGNAL(toggled(bool)), this, SLOT(toggleLightColors(bool)));
	
	actionSnps = new QAction(tr("&Variants"), this);
	actionSnps->setShortcut(QKeySequence("Ctrl+V"));
	actionSnps->setCheckable(true);
	menuWindow->addAction(actionSnps);
	
	actionSearch = new QAction(tr("&Find"), this);
	actionSearch->setShortcut(QKeySequence("Ctrl+F"));
	actionSearch->setCheckable(true);
	menuWindow->addAction(actionSearch);
	
	QAction * actionHelp = new QAction(tr("Gingr &Help"), this);
	actionHelp->setShortcut(QKeySequence("F1"));
	menuHelp->addAction(actionHelp);
	connect(actionHelp, SIGNAL(triggered()), this, SLOT(menuActionHelp()));
	snapshotWindow = new SnapshotWindow(this);
	connect(snapshotWindow, SIGNAL(signalSnapshot(const QString &, bool, bool)), this, SLOT(saveSnapshot(const QString &, bool, bool)));
	
//	menuBar->addAction(actionSnps);
	connect(actionSnps, SIGNAL(toggled(bool)), this, SLOT(toggleSnps(bool)));
	connect(actionSearch, SIGNAL(toggled(bool)), this, SLOT(toggleSearch(bool)));
	QVBoxLayout * layout = new QVBoxLayout();
	layout->setContentsMargins(5, 5, 5, 5);
	LinkedSplitter * splitterMain = new LinkedSplitter();
	QSplitter * splitterTree = new QSplitter();
	LinkedSplitter * splitterTop = new LinkedSplitter();
	
	treeViewMain = new PhylogenyTreeViewMain();
	nameListView = new NameListView();
//	alignmentView = new AlignmentView(240, 180);
//	alignmentView2 = new AlignmentView(270, 300);
//	alignmentView3 = new AlignmentView(130, 80);
	annotationView = new AnnotationView();
	blockViewMain = new BlockViewMain();
	rulerView = new RulerView();
	blockViewMap = new BlockViewMap();
	treeViewMap = new PhylogenyTreeViewMap();
	lcbView = new LcbView();
	referenceView = new ReferenceView();
	
	connectTrackListView(treeViewMain);
	//connectTrackListView(alignmentView);
	connectTrackListView(blockViewMain);
	//connect(alignmentView, SIGNAL(signalLcbHoverChange(int, float)), blockViewMain, SLOT(setLcbHover(int, float)));
	//connect(blockViewMain, SIGNAL(signalLcbHoverChange(int, float)), alignmentView, SLOT(setLcbHover(int, float)));
	
	connect(splitterTop, SIGNAL(splitterMoved(int, int)), splitterMain, SLOT(moveSplitter(int, int)));
	connect(splitterMain, SIGNAL(splitterMoved(int, int)), splitterTop, SLOT(moveSplitter(int, int)));
	
	connect(treeViewMain, SIGNAL(signalTrackZoom(int, int)), this, SLOT(setTrackZoom(int, int)));
	connect(treeViewMain, SIGNAL(signalFocusNode(const PhylogenyNode *, bool)), treeViewMap, SLOT(setFocusNode(const PhylogenyNode *, bool)));
	
	resize(1600, 900);
	
	QList<int> sizesTree;
	//
	sizesTree.push_back(width() * 0.7);
	sizesTree.push_back(width() * 0.3);
	//
	splitterTree->addWidget(treeViewMain);
	//	splitterTree->addWidget(nameListView);
	splitterTree->setSizes(sizesTree);
	//	splitterTree->setStretchFactor(0, 2);
	//	splitterTree->setStretchFactor(1, 1);
	
	QList<int> sizesMain;
	//
	sizesMain.push_back(width() * 0.3);
	sizesMain.push_back(width() * 0.7);
	sizesMain.push_back(width() * 0.0);
//	sizesMain.push_back(width() * 0.1);
//	sizesMain.push_back(width() * 0);
	//
	
	QWidget * treeWidget = new QWidget();
	QWidget * blockWidget = new QWidget();
	
	QVBoxLayout * treeLayout = new QVBoxLayout();
	QVBoxLayout * blockLayout = new QVBoxLayout();
	
	treeStatus = new PhylogenyTreeStatusBar();
	blockStatus = new BlockStatusBar();
	
	treeStatus->setMaximumHeight(15);
	treeStatus->setMinimumHeight(15);
	
	blockStatus->setMaximumHeight(15);
	blockStatus->setMinimumHeight(15);
	
	treeLayout->setMargin(0);
	treeLayout->setSpacing(3);
	treeLayout->addWidget(splitterTree);
	treeLayout->addWidget(treeStatus);
	
	blockLayout->setMargin(0);
	blockLayout->setSpacing(3);
	blockLayout->addWidget(blockViewMain);
	blockLayout->addWidget(blockStatus);
	
	treeWidget->setLayout(treeLayout);
	blockWidget->setLayout(blockLayout);
	
	splitterMain->addWidget(treeWidget);
	splitterMain->addWidget(blockWidget);
//	splitterMain->addWidget(alignmentView);
//	splitterMain->addWidget(filterControl);
//	splitterMain->addWidget(alignmentView2);
//	splitterMain->addWidget(alignmentView3);
	splitterMain->setSizes(sizesMain);
	
//	lcbView->setMinimumHeight(12);
	referenceView->setMinimumHeight(15);
	QVBoxLayout * topInfoLayout = new QVBoxLayout();
	
	topInfoLayout->addWidget(annotationView, 1);
	topInfoLayout->addWidget(rulerView, 0);
//	topInfoLayout->addWidget(lcbView, 0);
	topInfoLayout->addWidget(referenceView, 0);
	topInfoLayout->setMargin(0);
	topInfoLayout->setSpacing(3);
	
	QHBoxLayout * overviewLayout = new QHBoxLayout();
	overviewLayout->setMargin(0);
	overviewLayout->setSpacing(3);
	overviewLayout->addWidget(treeViewMap, 2);
	overviewLayout->addWidget(blockViewMap, 3);
	
	QWidget * topInfo = new QWidget();
	topInfo->setLayout(topInfoLayout);
	rulerView->setMaximumHeight(14);
	rulerView->setMinimumHeight(14);
	rulerView->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed));
	QWidget * overview = new QWidget();
	overview->setLayout(overviewLayout);
	splitterTop->addWidget(overview);
	splitterTop->addWidget(topInfo);
//	splitterTop->addWidget(new QFrame());
//	splitterTop->addWidget(new QFrame());
	splitterTop->setSizes(sizesMain);
	splitterTop->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed));
	//splitterTop->resize(1000, 100);
	splitterTop->setMaximumHeight(69);
	splitterTop->setMinimumHeight(69);
	layout->addWidget(splitterTop);
	layout->addWidget(splitterMain);
	centralWidget()->setLayout(layout);
	
	filterControl = new FilterControl(this);
	searchControl = new SearchControl(this);
	
	connect(blockViewMain, SIGNAL(positionChanged(int)), this, SLOT(setPosition(int)));
	connect(blockViewMain, SIGNAL(windowChanged(int, int)), this, SLOT(setWindow(int, int)));
	connect(blockViewMain, SIGNAL(signalMouseWheel(int)), this, SLOT(zoomFromMouseWheel(int)));
	
	connect(blockViewMap, SIGNAL(positionChanged(int)), this, SLOT(setPosition(int)));
	connect(blockViewMap, SIGNAL(signalWindowChanged(int, int)), this, SLOT(setWindow(int, int)));
	connect(blockViewMap, SIGNAL(signalMouseWheel(int)), this, SLOT(zoomFromMouseWheel(int)));
	
	connect(annotationView, SIGNAL(positionChanged(int)), this, SLOT(setPosition(int)));
	connect(annotationView, SIGNAL(signalWindowTarget(int, int)), this, SLOT(setWindowTarget(int, int)));
	connect(annotationView, SIGNAL(signalMouseWheel(int)), this, SLOT(zoomFromMouseWheel(int)));
	
	connect(rulerView, SIGNAL(positionChanged(int)), this, SLOT(setPosition(int)));
	connect(rulerView, SIGNAL(signalWindowTarget(int, int)), this, SLOT(setWindowTarget(int, int)));
	connect(rulerView, SIGNAL(signalMouseWheel(int)), this, SLOT(zoomFromMouseWheel(int)));
	
	connect(referenceView, SIGNAL(positionChanged(int)), this, SLOT(setPosition(int)));
	connect(referenceView, SIGNAL(signalMouseWheel(int)), this, SLOT(zoomFromMouseWheel(int)));
	
	connect(filterControl, SIGNAL(filtersChanged()), blockViewMain, SLOT(updateSnpsNeeded()));
	connect(filterControl, SIGNAL(filtersChanged()), blockViewMap, SLOT(updateSnpsNeeded()));
	connect(treeViewMain, SIGNAL(signalNodeHover(const PhylogenyNode *)), this, SLOT(setNode(const PhylogenyNode *)));
	connect(filterControl, SIGNAL(closed()), this, SLOT(closeSnps()));
	connect(searchControl, SIGNAL(closed()), this, SLOT(closeSearch()));
	connect(searchControl, SIGNAL(signalSearchChanged(const QString &, bool)), treeViewMain, SLOT(search(const QString &, bool)));
	connect(treeViewMain, SIGNAL(signalSearchResults(int)), searchControl, SLOT(resultsChanged(int)));
	connect(&snpBufferMain, SIGNAL(updated()), this, SLOT(updateSnpsFinishedMain()));
	connect(&snpBufferMain, SIGNAL(updated()), blockViewMain, SLOT(updateSnpsFinished()));
	connect(&snpBufferMain, SIGNAL(updated()), referenceView, SLOT(updateSnpsFinished()));
	connect(&snpBufferMap, SIGNAL(updated()), blockViewMap, SLOT(updateSnpsFinished()));
	connect(blockViewMain, SIGNAL(signalUpdateSnps()), this, SLOT(updateSnpsMain()));
	connect(blockViewMap, SIGNAL(signalUpdateSnps()), this, SLOT(updateSnpsMap()));
	connect(blockViewMain, SIGNAL(signalToggleSynteny()), this, SLOT(toggleSynteny()));
	//filterControl->setParent(this);
	
//	OptionButton * filterButton = new OptionButton(filterControl, "Snps");
	QHBoxLayout * buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch();
//	buttonLayout->addWidget(filterButton, 0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
//	buttonLayout->setSpacing(0);
	
//	layout->addLayout(buttonLayout);
	layout->setSpacing(3);
	
	show();
	
	QTimer * timer = new QTimer(this); // TODO: delete?
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(20);
	
	setAutoFillBackground(false);
	/*
	QMessageBox msgBox;
	char text[1024];
	sprintf(text, "%d", argc);
	msgBox.setText(text);
	msgBox.exec();
	for ( int i = 0; i < argc; i++ )
	{
		printf("arg %d: %s\n", i, argv[i]);
		QMessageBox msgBox;
		msgBox.setText(argv[i]);
		msgBox.exec();
	}
	*/
	if ( argc > 1 )
	{
		int i = 1;
		while ( i < argc && argv[i][0] == '-' )
		{
			i += 2;
		}
		
		if ( i < argc )
		{
			loadPb(QString(argv[i]));
		}
		//loadXml(QString(argv[1]));
	}
	
	actionToggleLightColors->setChecked(true);
}

MainWindow::~MainWindow()
{
	delete [] trackHeights;
	delete [] trackHeightsOverview;
	
	if ( phylogenyTree )
	{
		delete phylogenyTree;
	}
}

void MainWindow::closeSnps()
{
	actionSnps->setChecked(false);
}

void MainWindow::closeSearch()
{
	actionSearch->setChecked(false);
}

void MainWindow::menuActionHelp()
{
	if ( help == 0)
	{
		help = new QWidget(this);
		
		QTextBrowser * text = new QTextBrowser(help);
		text->setSource(QUrl("qrc:/html/index.html"));
		text->setFrameStyle(QFrame::NoFrame);
		
		QHBoxLayout * layout = new QHBoxLayout();
		layout->setMargin(0);
		layout->addWidget(text);
		
		help->setLayout(layout);
		help->setWindowFlags(Qt::Window);
		help->resize(800, 800);
		help->setWindowTitle(tr("Gingr - Help"));
		help->show();
	}
	
	help->setVisible(true);
	help->raise();
}

void MainWindow::menuImportAlignment()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open alignment file"), ".", tr("Multi-fasta (*.mfa *.fasta *.fna)"));
	
	if ( ! fileName.isNull() )
	{
		loadAlignment(fileName);
	}
}

void MainWindow::menuImportAnnotations()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open annotation file"), ".", tr("Genbank (*.gb *.gbk)"));
	
	if ( ! fileName.isNull() )
	{
		loadAnnotations(fileName);
	}
}

void MainWindow::menuImportTree()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open tree file"), ".", tr("Newick (*.tre *.tree *.nwk)"));
	
	if ( ! fileName.isNull() )
	{
		loadTree(fileName);
	}
}

void MainWindow::menuOpen()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Harvest file"), ".", tr("Harvest Files (*.hvt)"));
	
	if ( ! fileName.isNull() )
	{
		loadPb(fileName);
	}
}

void MainWindow::menuSnapshot()
{
	snapshotWindow->show();
}

void MainWindow::toggleSynteny()
{
	synteny = ! synteny;
	actionToggleSynteny->setChecked(synteny);
	updateSnpsMain();
	updateSnpsMap();
	blockStatus->setSynteny(synteny);
}

void MainWindow::toggleLightColors(bool checked)
{
	lightColors = checked;
	blockViewMain->setLightColors(lightColors);
	blockViewMap->setLightColors(lightColors);
	blockStatus->setLightColors(lightColors);
	referenceView->setLightColors(lightColors);
	updateSnpsMain();
	updateSnpsMap();
}

void MainWindow::toggleSnps(bool checked)
{
	if ( checked )
	{
		filterControl->restore();
	}
	else
	{
		filterControl->minimize(0, 0);
	}
}

void MainWindow::toggleSearch(bool checked)
{
	if ( checked )
	{
		searchControl->restore();
	}
	else
	{
		searchControl->minimize(0, 0);
	}
}

void MainWindow::toggleRightAlignNodes(bool checked)
{
	treeViewMain->setRightAlignNodes(checked);
}

void MainWindow::toggleRightAlignText(bool checked)
{
	treeViewMain->setRightAlignText(checked);
}

void MainWindow::saveSnapshot(const QString & fileName, bool tree, bool alignment)
{
	int width = 0;
	int height = treeViewMain->getHeight();
	
	if ( tree )
	{
		width += treeViewMain->getWidth();
	}
	
	if ( alignment )
	{
		width += blockViewMain->getWidth();
	}
	
	QPixmap pixmap(width, height);
	QPainter painter(&pixmap);
	
	int x = 0;
	
	if ( tree )
	{
		painter.drawImage(0, 0, *treeViewMain->getBuffer());
		x += treeViewMain->getWidth();
	}
	
	if ( alignment )
	{
		QPixmap pixmapAlignment = QPixmap::grabWidget(blockViewMain, 0, 0);
		painter.drawImage(x, 0, *blockViewMain->getBuffer());
	}
	
	pixmap.save(fileName);
}

void MainWindow::setNode(const PhylogenyNode *node)
{
	if (node->getChildrenCount() )
	{
		treeStatus->setPhylogenyNode(node, tr(""));
	}
	else
	{
		treeStatus->setPhylogenyNode(node, names[leafIds[node->getLeafMin()]]);
	}
}

void MainWindow::setPosition(int gapped)
{
	int abs;
	int offset;
	
	if ( gapped != -1 )
	{
		Alignment::Position ungapped = alignment.getPositionUngapped(gapped);
		abs = ungapped.abs;
		offset = ungapped.gap;
	}
	else
	{
		abs = -1;
		offset = 0;
	}
	
	posCursor = gapped;
	blockStatus->setPosition(abs, offset);
	rulerView->setPosition(gapped, abs, offset);
	annotationView->setPosition(gapped, abs, offset);
}

void MainWindow::setTrackFocus(int track)
{
	trackFocusLast = trackFocus;
	trackFocus = track;
	trackZoomStartLast = trackZoomStart;
	trackZoomEndLast = trackZoomEnd;
	
	timerFocus.initialize(325);
	treeViewMain->setTrackFocus(trackFocus);
//	alignmentView->setTrackFocus(trackFocus);
	blockViewMain->setTrackFocus(trackFocus);
}

void MainWindow::setTrackHover(int track, int trackEnd)
{
	//if ( trackListViewFocus != (TrackListView *)QObject::sender() ) return;
	treeViewMain->setTrackHover(track, trackEnd);
//	nameListView->setHighlightTrack(track);
//	alignmentView->setTrackHover(track, trackEnd);
	blockViewMain->setTrackHover(track, trackEnd);
	
	if ( track == trackEnd )
	{
		if ( track == -1 )
		{
			treeStatus->setPhylogenyNode(0, tr(""), track);
		}
		else
		{
			if ( phylogenyTree )
			{
				treeStatus->setPhylogenyNode(phylogenyTree->getLeaf(track), names[leafIds[track]], track);
			}
			else
			{
				treeStatus->setName(names[track]);
			}
		}
	}
}

void MainWindow::setTrackListViewFocus(TrackListView * view)
{
	trackListViewFocus = view;
}

void MainWindow::setTrackZoom(int start, int end)
{
	if ( start != trackZoomStart || end != trackZoomEnd )
	{
		trackZoomStartLast = trackZoomStart;
		trackZoomEndLast = trackZoomEnd;
		trackZoomStart = start;
		trackZoomEnd = end;
		trackFocusLast = trackFocus;
		timerFocus.initialize(325);
	}
	
//	blockViewMap->setTrackZoom(start, end);
}

void MainWindow::setWindow(int start, int end)
{
	posStart = start;
	posEnd = end;
	zoom = (float)alignment.getLength() / (end - start + 1);
	
	annotationView->setWindow(start, end);
	rulerView->setWindow(start, end);
	blockViewMain->setWindow(start, end);
	blockViewMap->setWindow(start, end);
	//lcbView->setWindow(start, end);
	referenceView->setWindow(start, end);
	//blockStatus->setLegendBases((end - start + 1) / blockViewMain->getWidth() < 1);
}

void MainWindow::setWindowTarget(int start, int end)
{
	int width = blockViewMain->getWidth();
	
	if ( (end - start + 1) * 12 < width )
	{
		int center = (start + end) / 2;
		
		start = center - width / 12 / 2;
		end = center + width / 12 / 2;
	}
	
	if ( start < 0 )
	{
		end = end - start;
		start = 0;
	}
	
	if ( end >= alignment.getLength() )
	{
		start = start - (end - alignment.getLength() + 1);
		end = alignment.getLength() - 1;
	}
	
	windowTargetStart = start;
	windowTargetEnd = end;
	
	tweenWindowStart.initialize(float(start - posStart + 1) / (posEnd - posStart + 1), 0.);
	tweenWindowEnd.initialize(float(end - posStart + 1) / (posEnd - posStart + 1), 1.);
	
	timerWindow.initialize(750);
}

void MainWindow::unsetTrackListViewFocus(TrackListView *view)
{
	if ( trackListViewFocus == view )
	{
//		setTrackHover(-1);
		trackListViewFocus = 0;
	}
}

void MainWindow::update()
{
	bool timerFocusUpdated = timerFocus.update();
	bool timerWindowUpdated = timerWindow.update();
	
	if ( trackCount && timerFocusUpdated )
	{
		updateTrackHeights();
		
		treeViewMain->setZoomProgress(timerFocus.getProgress());
		treeViewMap->setZoomProgress(timerFocus.getProgress());
		
		if ( trackListViewFocus )
		{
//			trackListViewFocus->handleTrackHeightChange();
		}
//		treeViewMain->handleTrackHeightChange(trackListViewFocus);
//		blockViewMain->handleTrackHeightChange(trackListViewFocus);
//		alignmentView->handleTrackHeightChange(trackListViewFocus);
	}
	
	if ( trackCount && timerWindowUpdated )
	{
		timerWindow.update();
		
		tweenWindowStart.update(timerWindow.getProgress());
		tweenWindowEnd.update(timerWindow.getProgress());
		
		int windowSize = (windowTargetEnd - windowTargetStart + 1) / (tweenWindowEnd.getValue() - tweenWindowStart.getValue());
		int start = windowTargetStart - windowSize * tweenWindowStart.getValue();
		int end = windowTargetStart + windowSize * (1 - tweenWindowStart.getValue());
		
		setWindow(start, end);
	}
	/*
	if ( alignmentView->getTweenNeeded() || alignmentView2->getTweenNeeded() || alignmentView3->getTweenNeeded() )
	{
		timerAlignment.initialize(1250);
	}
	*/
	treeViewMain->update();
	//nameListView->update();
	
//	alignmentView->update(timerAlignment.getProgress());
//	alignmentView2->update(timerAlignment.getProgress());
//	alignmentView3->update(timerAlignment.getProgress());
	blockViewMain->update();
	blockViewMap->update();
	annotationView->update();
	rulerView->update();
	treeViewMap->update();
}

void MainWindow::updateSnpsFinishedMain()
{
	blockStatus->setLegendBases(snpBufferMain.getMax() <= 1);
}

void MainWindow::updateSnpsMain()
{
	snpBufferMain.update(posStart * 2 - posEnd - 1, 2 * posEnd - posStart + 1, 3 * blockViewMain->getWidth(), synteny, lightColors);
}

void MainWindow::updateSnpsMap()
{
	snpBufferMap.update(0, alignment.getLength(), blockViewMap->getWidth(), synteny, lightColors);
}

void MainWindow::zoomFromMouseWheel(int delta)
{
	float zoomLast = zoom;
	float zoomFactor = 1 + qAbs((float)delta) / 400;
	
	if ( delta > 0 )
	{
		zoom *= zoomFactor;
	}
	else
	{
		zoom /= zoomFactor;
	}
	
	if ( zoom < 1 )
	{
		zoom = 1;
	}
	
	int refSize = alignment.getLength();
	int width = blockViewMain->getWidth();
	float zoomMax = 12. * refSize / width;
	
	if ( zoom > zoomMax )
	{
		zoom = zoomMax;
	}
	
	if ( zoom == zoomLast )
	{
		return;
	}
	
	int sizeCur = (posEnd - posStart + 1);
	int size = refSize / zoom;
	int left = long(posCursor - posStart) * size / sizeCur;
	int right = size - left;
	
	posStart = posCursor - left;
	posEnd = posCursor + right - 1;
	
	if ( posStart < 0 )
	{
		posStart = 0;
		posEnd = size - 1;
	}
	
	if ( posEnd >= refSize )
	{
		posEnd = refSize;
		posStart = posEnd - size + 1;
	}
	
	setWindow(posStart, posEnd);
	//printf("Zoom: %f\t%d\t%d\t%d\n", zoom, posStart, focus, posEnd);
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
	if ( trackCount )
	{
		updateTrackHeights();
		updateTrackHeightsOverview();
	}
}

void MainWindow::connectTrackListView(TrackListView *view)
{
	connect(view, SIGNAL(signalTrackHoverChange(int, int)), this, SLOT(setTrackHover(int, int)));
	connect(view, SIGNAL(signalTrackFocusChange(int)), this, SLOT(setTrackFocus(int)));
	connect(view, SIGNAL(signalFocus(TrackListView *)), this, SLOT(setTrackListViewFocus(TrackListView *)));
	connect(view, SIGNAL(signalUnfocus(TrackListView *)), this, SLOT(setTrackListViewFocus(TrackListView *)));
}

void MainWindow::initialize()
{
	snpBufferMain.initialize(&alignment);
	snpBufferMap.initialize(&alignment);
	filterControl->setAlignment(&alignment);
	
	phylogenyTree->getLeafIds(leafIds);
	trackHeights = new float[leafIds.size() + 1];
	trackHeightsOverview = new float[leafIds.size() + 1];
	trackCount = leafIds.size();
	trackZoomStart = 0;
	trackZoomEnd = trackCount - 1;
	trackZoomStartLast = 0;
	trackZoomEndLast = trackZoomEnd;
	tweenYFactor.initialize(0, 0);
	tweenYOffset.initialize(0, 0);
	timerFocus.initialize(0);
	setTrackZoom(trackZoomStart, trackZoomEnd);
	
	treeViewMain->setTrackHeights(trackHeights, trackCount);
	treeViewMain->setIdByTrack(&leafIds);
	treeViewMain->setPhylogenyTree(phylogenyTree);
	treeViewMain->setNames(&names);
	treeViewMap->setTrackHeights(trackHeightsOverview, trackCount);
	treeViewMap->setIdByTrack(&leafIds);
	treeViewMap->setPhylogenyTree(phylogenyTree);
	treeViewMap->setNames(&names);
	//	treeViewMain->setNames(&labels);
	
	//	nameListView->setOrder(&leafIds);
	//	nameListView->setNames(&names);
	//	nameListView->setTrackHeights(trackHeights, leafIds.size());
	/*
	 alignmentView->setIdByTrack(&leafIds);
	 alignmentView->setAlignment(&alignment);
	 alignmentView->setTrackHeights(trackHeights, trackCount);
	 
	 alignmentView2->setIdByTrack(&leafIds);
	 alignmentView2->setAlignment(&alignment);
	 alignmentView->setTrackHeights(trackHeights, trackCount);
	 
	 alignmentView3->setIdByTrack(&leafIds);
	 alignmentView3->setAlignment(&alignment);
	 alignmentView->setTrackHeights(trackHeights, trackCount);
	 */
	//lcbView->setAlignment(&alignment);
	rulerView->setAlignment(&alignment);
	referenceView->setAlignment(&alignment);
	referenceView->setSnpBuffer(&snpBufferMain);
	
	leafDists = new float[trackCount];
	leafDists[0] = 0;
	float maxDist = 0;
	/*
	for ( int i = 1; i < trackCount; i++ )
	{
		leafDists[i] = phylogenyTree->leafDistance(i - 1, i);
		
		if ( leafDists[i] > maxDist )
		{
			maxDist = leafDists[i];
		}
	}
	
	float factor = 3. / maxDist;
	
	for ( int i = 0; i < trackCount; i++ )
	{
		leafDists[i] = 0 + factor * leafDists[i];
	}
	*/
	blockViewMain->setLeafDists(leafDists);
	blockViewMain->setIdByTrack(&leafIds);
	blockViewMain->setTrackHeights(trackHeights, trackCount);
	blockViewMain->setAlignment(&alignment);
	blockViewMain->setSnpBuffer(&snpBufferMain);
	blockViewMap->setLeafDists(leafDists);
	blockViewMap->setIdByTrack(&leafIds);
	blockViewMap->setTrackHeights(trackHeightsOverview, trackCount);
	blockViewMap->setAlignment(&alignment);
	blockViewMap->setSnpBuffer(&snpBufferMap);
	
	searchControl->initialize();
	
	updateTrackHeightsOverview();
	
	blockStatus->setShowLegend(true);
}

void MainWindow::initializeAlignment()
{
	snpBufferMain.initialize(&alignment);
	snpBufferMap.initialize(&alignment);
	
	trackCount = hio.harvest.tracks().tracks_size();
	leafIds.resize(trackCount);
	
	for ( int i = 0; i < trackCount; i++ )
	{
		leafIds[i] = i;
	}
	//tree.getLeafIds(leafIds);
	
	trackHeights = new float[leafIds.size() + 1];
	trackHeightsOverview = new float[leafIds.size() + 1];
	trackZoomStart = 0;
	trackZoomEnd = trackCount - 1;
	trackZoomStartLast = 0;
	trackZoomEndLast = trackZoomEnd;
	tweenYFactor.initialize(0, 0);
	tweenYOffset.initialize(0, 0);
	timerFocus.initialize(0);
	setTrackZoom(trackZoomStart, trackZoomEnd);
	
	treeViewMain->setTrackHeights(trackHeights, trackCount);
	treeViewMain->setIdByTrack(&leafIds);
	//treeViewMain->setPhylogenyTree(&tree);
	treeViewMain->setNames(&names);
	treeViewMap->setTrackHeights(trackHeightsOverview, trackCount);
	treeViewMap->setIdByTrack(&leafIds);
	//treeViewMap->setPhylogenyTree(&tree);
	treeViewMap->setNames(&names);
	//	treeViewMain->setNames(&labels);
	
	rulerView->setAlignment(&alignment);
	referenceView->setAlignment(&alignment);
	referenceView->setSnpBuffer(&snpBufferMain);
	
	/*
	leafDists = new float[trackCount];
	leafDists[0] = 0;
	float maxDist = 0;
	
	for ( int i = 1; i < trackCount; i++ )
	{
		leafDists[i] = tree.leafDistance(i - 1, i);
		
		if ( leafDists[i] > maxDist )
		{
			maxDist = leafDists[i];
		}
	}
	
	float factor = 3. / maxDist;
	
	for ( int i = 0; i < trackCount; i++ )
	{
		leafDists[i] = 0 + factor * leafDists[i];
	}
	*/
	
	//blockViewMain->setLeafDists(leafDists);
	blockViewMain->setIdByTrack(&leafIds);
	blockViewMain->setTrackHeights(trackHeights, trackCount);
	blockViewMain->setAlignment(&alignment);
	blockViewMain->setSnpBuffer(&snpBufferMain);
	//blockViewMap->setLeafDists(leafDists);
	blockViewMap->setIdByTrack(&leafIds);
	blockViewMap->setTrackHeights(trackHeightsOverview, trackCount);
	blockViewMap->setAlignment(&alignment);
	blockViewMap->setSnpBuffer(&snpBufferMap);
	
	filterControl->setAlignment(&alignment);
	searchControl->initialize();
	
	updateTrackHeightsOverview();
	
	blockStatus->setShowLegend(true);
}

void MainWindow::initializeTree()
{
	phylogenyTree->getLeafIds(leafIds);
	treeViewMain->setPhylogenyTree(phylogenyTree);
	treeViewMap->setPhylogenyTree(phylogenyTree);
	treeViewMain->setNames(&names);
	treeViewMap->setNames(&names);
	searchControl->initialize();
	
	
	if ( ! trackHeights )
	{
		trackCount = leafIds.size();
		trackHeights = new float[leafIds.size() + 1];
		trackHeightsOverview = new float[leafIds.size() + 1];
		trackZoomStart = 0;
		trackZoomEnd = trackCount - 1;
		trackZoomStartLast = 0;
		trackZoomEndLast = trackZoomEnd;
		tweenYFactor.initialize(0, 0);
		tweenYOffset.initialize(0, 0);
		timerFocus.initialize(0);
		setTrackZoom(trackZoomStart, trackZoomEnd);
		treeViewMain->setTrackHeights(trackHeights, trackCount);
		treeViewMain->setIdByTrack(&leafIds);
		treeViewMap->setTrackHeights(trackHeightsOverview, trackCount);
		treeViewMap->setIdByTrack(&leafIds);
	}
}

void MainWindow::loadAlignment(const QString &fileName)
{
	QFileInfo fileInfo(fileName);
	QProgressDialog dialog;
	dialog.setCancelButton(0);
	dialog.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	dialog.setLabelText(QString("Loading %1...").arg(fileInfo.fileName()));
	QFutureWatcher<void> futureWatcher;
	QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
	QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
	futureWatcher.setFuture(QtConcurrent::run(this, &MainWindow::loadAlignmentBackground, fileName));
	
	printf("%s\n", fileName.toStdString().c_str());
	
	// Display the dialog and start the event loop.
	dialog.exec();
	
	futureWatcher.waitForFinished();
	
	initializeAlignment();
}

void MainWindow::loadAlignmentBackground(const QString &fileName)
{
	printf("%s\n", fileName.toStdString().c_str());
	
	hio.loadMFA(fileName.toStdString().c_str());
	loadPbNames(hio.harvest.tracks());
//	tree.loadPb(hio.harvest.tree());
	alignment.loadPb(hio.harvest.alignment(), hio.harvest.variation(), hio.harvest.reference(), hio.harvest.tracks().tracks_size());
	annotationView->setAlignment(&alignment);
}

void MainWindow::loadAnnotations(const QString &fileName)
{
	hio.loadGenbank(fileName.toStdString().c_str());
	annotationView->loadPb(hio.harvest.annotations());
}

bool MainWindow::loadPb(const QString & fileName)
{
	QFileInfo fileInfo(fileName);
	QProgressDialog dialog;
	dialog.setCancelButton(0);
	dialog.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	dialog.setLabelText(QString("Loading %1...").arg(fileInfo.fileName()));
	
	// Create a QFutureWatcher and connect signals and slots.
	QFutureWatcher<void> futureWatcher;
	QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
//	QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
	QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
//	QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
	
	// Start the computation.
	futureWatcher.setFuture(QtConcurrent::run(this, &MainWindow::loadPbBackground, fileName));
	
	// Display the dialog and start the event loop.
	dialog.exec();
	
	futureWatcher.waitForFinished();
	
	if ( futureWatcher.isCanceled() )
	{
		names.resize(0);
		labels.resize(0);
		return false;
	}
	
	initialize();
	setWindowTitle(tr("Gingr - ").append(fileInfo.fileName()));
	return true;
}

void MainWindow::loadPbBackground(const QString &fileName)
{
	if ( ! hio.loadHarvest(fileName.toStdString().c_str()) )
	{
		printf("FAILED to load %s", fileName.toStdString().c_str());
	}
	
	loadPbNames(hio.harvest.tracks());
	
	if ( phylogenyTree )
	{
		delete phylogenyTree;
	}
	
	phylogenyTree = new PhylogenyTree();
	phylogenyTree->loadPb(hio.harvest.tree());
	
	alignment.loadPb(hio.harvest.alignment(), hio.harvest.variation(), hio.harvest.reference(), hio.harvest.tracks().tracks_size());
	annotationView->setAlignment(&alignment);
	
	if ( hio.harvest.has_annotations() )
	{
		annotationView->loadPb(hio.harvest.annotations());
	}
}

bool MainWindow::loadPbNames(const Harvest::TrackList & msg)
{
	names.resize(msg.tracks_size());
	labels.resize(msg.tracks_size());
	
	for ( int i = 0; i < msg.tracks_size(); i++ )
	{
		const Harvest::TrackList::Track & msgTrack = msg.tracks(i);
		
		if ( msgTrack.has_name() )
		{
			labels[i] = QString::fromStdString(msgTrack.name());
		}
		
		names[i] = QString::fromStdString(msgTrack.file());
	}
	
	return true;
}

void MainWindow::loadTree(const QString & fileName)
{
	if ( phylogenyTree )
	{
		delete phylogenyTree;
	}
	
	phylogenyTree = new PhylogenyTree();
	hio.loadNewick(fileName.toStdString().c_str());
	phylogenyTree->loadPb(hio.harvest.tree());
	loadPbNames(hio.harvest.tracks());
	initializeTree();
}

bool MainWindow::loadXml(const QString & fileName)
{
	setWindowTitle(tr("Gingr - ").append(fileName));
	
	QDomDocument domDocument;
	QFile file(fileName);
	
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		/*		QMessageBox::warning(this, QObject::tr("SAX Bookmarks"),
		 QObject::tr("Cannot read file %1:\n%2.")
		 .arg(fileName)
		 .arg(file.errorString()));
		 */		return false;
	}
	
	if ( ! domDocument.setContent(&file) )
	{
		return false;
	}
	
	QDomElement documentElement = domDocument.documentElement();
	
	QDomElement namesElement = documentElement.firstChildElement("tracks");
	loadDomNames(&namesElement);
	
	QDomElement treeElement = documentElement.firstChildElement("node");
	phylogenyTree->loadDom(&treeElement);
	
	alignment.loadDom(&documentElement);
	
	annotationView->setAlignment(&alignment);
	QDomElement annotationElement = documentElement.firstChildElement("annotations");
	if ( ! annotationElement.isNull() )
	{
		annotationView->loadDom(&annotationElement);
	}
	
	return true;
}

bool MainWindow::loadDomNames(const QDomElement * elementNames)
{
	for( QDomElement element = elementNames->firstChildElement("track"); ! element.isNull(); element = element.nextSiblingElement("track") )
	{
		int i = element.attribute("id").toInt();
		
		if ( names.size() <= i )
		{
			names.resize(i + 1);
		}
		
		if ( element.hasAttribute("label") )
		{
			if ( labels.size() <= i )
			{
				labels.resize(i + 1);
			}
			
			labels[i] = element.attribute("label");
		}
		
		names[i] = element.firstChild().nodeValue();
		i++;
	}
	
	return true;
}

void MainWindow::updateTrackHeights(bool setTargets)
{
	int radius = 4;
	float trackHeightLast = (float)(treeViewMain->getHeight() - 150) / (trackZoomEndLast - trackZoomStartLast + 1);
	trackHeights[0] = 0;
	
	float zoomHeights[radius * 2 + 1];
	
	for ( int i = 0; i < radius * 2 + 1; i++ )
	{
		int distance;
		
		if ( i <= radius )
		{
			distance = radius - i;
		}
		else
		{
			distance = i - radius;
		}
		
		zoomHeights[i] = cos(distance * 3.1416 / radius / 2);
	}
	
	float integral = 0;
	int integralCount = 0;
	
	for ( int i = trackFocus - radius; i <= trackFocus + radius; i++ )
	{
		if ( i >= trackZoomStart && i <= trackZoomEnd )
		{
			integral += zoomHeights[i - trackFocus + radius];
			integralCount++;
		}
	}
	
	float integralLast = 0;
	
	int trackFocusZoomLast = trackZoomStart == trackZoomStartLast && trackZoomEnd == trackZoomEndLast ? trackFocusLast : trackFocus;
	
	for ( int i = trackFocusZoomLast - radius; i <= trackFocusZoomLast + radius; i++ )
	{
		if ( i >= trackZoomStartLast && i <= trackZoomEndLast )
		{
			integralLast += zoomHeights[i - trackFocusZoomLast + radius];
		}
	}
	
	float maxZoomHeight = 19;
	int tracks = trackZoomEnd - trackZoomStart + 1;
	int tracksLast = trackZoomEndLast - trackZoomStartLast + 1;
	int viewHeight = treeViewMain->getHeight();
	float scale = (maxZoomHeight * tracks - viewHeight) / (tracks - integral);
	float scaleLast = (maxZoomHeight * tracksLast - viewHeight) / (tracksLast - integralLast);
	float trackHeight;
	
	if ( scale < 0 )
	{
		scale = 0;
	}
	
	if ( scaleLast < 0 )
	{
		scaleLast = 0;
	}
	
	if ( scale * integral > viewHeight / 2 )
	{
		scale = viewHeight / 2 / integral;
	}
	
	if ( scaleLast * integral > viewHeight / 2 )
	{
		scaleLast = viewHeight / 2 / integral;
	}
	
	if ( trackZoomStart == trackZoomStartLast && trackZoomEnd == trackZoomEndLast )
	{
		trackHeight = maxZoomHeight - scale;
		scale *= timerFocus.getProgress();
	}
	else
	{
		scale = scaleLast + timerFocus.getProgress() * (scale - scaleLast);
		trackHeight = maxZoomHeight - scale;
	}
	
	for ( int i = 1; i < trackCount + 1; i++ )
	{
		trackHeights[i] = trackHeights[i - 1] + trackHeight;
		
		int distance;
		
		if ( i <= trackFocus )
		{
			distance = trackFocus - i + 1;
		}
		else
		{
			distance = i - trackFocus - 1;
		}
		
		if ( distance <= radius && trackFocus != -1 )
		{
			trackHeights[i] += scale * zoomHeights[i - trackFocus + radius - 1];
		}
		
		int distanceLast;
		
		if ( i <= trackFocusLast )
		{
			distanceLast = trackFocusLast - i + 1;
		}
		else
		{
			distanceLast = i - trackFocusLast - 1;
		}
		
		if ( trackFocusLast != -1 && distanceLast <= radius && trackZoomStart == trackZoomStartLast && trackZoomEnd == trackZoomEndLast )
		{
			trackHeights[i] += scaleLast * (1 - timerFocus.getProgress()) * zoomHeights[i - trackFocusLast + radius - 1];
//			trackHeights[i] += 5 * (1 - timerFocus.getProgress()) * cos(distanceLast * 3.1416 / radius / 2);
		}
	}
	
	float progress = timerFocus.getProgress();
	
	float yFactorPrev = (viewHeight - 1) / (trackHeights[trackZoomEndLast + 1] - trackHeights[trackZoomStartLast]);
	float yFactorTarget = (viewHeight - 1) / (trackHeights[trackZoomEnd + 1] - trackHeights[trackZoomStart]);
	float yOffsetPrev = -yFactorPrev * trackHeights[trackZoomStartLast];
	float yOffsetTarget = -yFactorTarget * trackHeights[trackZoomStart];
	float yFactor = yFactorPrev + progress * (yFactorTarget - yFactorPrev);
	float yOffset = yOffsetPrev + progress * (yOffsetTarget - yOffsetPrev);
	
	float mapTopPrev = trackHeightsOverview[trackZoomStartLast];
	float mapTop = trackHeightsOverview[trackZoomStart];
	float mapBottomPrev = trackHeightsOverview[trackZoomEndLast + 1];
	float mapBottom = trackHeightsOverview[trackZoomEnd + 1];
	
	blockViewMap->setTrackZoom(mapTopPrev + progress * (mapTop - mapTopPrev), mapBottomPrev + progress * (mapBottom - mapBottomPrev));
	//tweenYFactor.update(timerTrackZoom.getProgress());
	//tweenYOffset.update(timerTrackZoom.getProgress());
	
	for ( int i = 0; i < trackCount + 1; i++ )
	{
		trackHeights[i] = trackHeights[i] * yFactor + yOffset;
	}
	
	treeViewMain->handleTrackHeightChange(trackListViewFocus);
	blockViewMain->handleTrackHeightChange(trackListViewFocus);
	//alignmentView->handleTrackHeightChange(trackListViewFocus);
	//	emit heightsChanged();
}

void MainWindow::updateTrackHeightsOverview()
{
	float factor = (float)blockViewMap->getHeight() / trackCount;
	
	for ( int i = 0; i < trackCount + 1; i++ )
	{
		trackHeightsOverview[i] = i * factor;
	}
}
