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
#include "FilterControl.h"
#include "OptionButton.h"

MainWindow::MainWindow(int argc, char ** argv, QWidget * parent)
: QWidget(parent)
{
	trackFocus = -1;
	trackFocusLast = -1;
	trackListViewFocus = 0;
	
	QVBoxLayout * layout = new QVBoxLayout();
	layout->setContentsMargins(5, 5, 5, 5);
	LinkedSplitter * splitterMain = new LinkedSplitter();
	QSplitter * splitterTree = new QSplitter();
	LinkedSplitter * splitterTop = new LinkedSplitter();
	
	treeView = new PhylogenyTreeView();
	nameListView = new NameListView();
	alignmentView = new AlignmentView(240, 180);
	alignmentView2 = new AlignmentView(270, 300);
	alignmentView3 = new AlignmentView(130, 80);
	annotationView = new AnnotationView();
	blockViewMain = new BlockViewMain();
	rulerView = new RulerView();
	blockViewMap = new BlockViewMap();
	PhylogenyTreeView * treeOverview = new PhylogenyTreeView();
	lcbView = new LcbView();
	referenceView = new ReferenceView();
	
	connectTrackListView(treeView);
	connectTrackListView(alignmentView);
	connectTrackListView(blockViewMain);
	connect(alignmentView, SIGNAL(signalLcbHoverChange(int, float)), blockViewMain, SLOT(setLcbHover(int, float)));
	connect(blockViewMain, SIGNAL(signalLcbHoverChange(int, float)), alignmentView, SLOT(setLcbHover(int, float)));
	
	connect(splitterTop, SIGNAL(splitterMoved(int, int)), splitterMain, SLOT(moveSplitter(int, int)));
	connect(splitterMain, SIGNAL(splitterMoved(int, int)), splitterTop, SLOT(moveSplitter(int, int)));
	
	connect(treeView, SIGNAL(signalTrackZoom(int, int)), this, SLOT(setTrackZoom(int, int)));
	
	if ( argc > 3 )
	{
		loadXml(QString(argv[1]));
	}
	else
	{
		QString fileName = QFileDialog::getOpenFileName(this,
														tr("Open XML"), ".", tr("XML Files (*.xml)"));
		loadXml(fileName);
	}
	
	tree.getLeafIds(leafIds);
	trackHeights = new float[leafIds.size() + 1];
	trackHeightsOverview = new float[leafIds.size() + 1];
	trackCount = leafIds.size();
	trackZoomStart = 0;
	trackZoomEnd = trackCount - 1;
	trackZoomStartLast = 0;
	trackZoomEndLast = trackZoomEnd;
	tweenYFactor.initialize(0, 0);
	tweenYOffset.initialize(0, 0);
	timerTrackZoom.initialize(0);
	setTrackZoom(trackZoomStart, trackZoomEnd);
	
	resize(1600, 900);
	
	QList<int> sizesTree;
	//
	sizesTree.push_back(width() * 0.7);
	sizesTree.push_back(width() * 0.3);
	//
	splitterTree->addWidget(treeView);
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
	
	lcbView->setMinimumHeight(12);
	referenceView->setMinimumHeight(12);
	QVBoxLayout * topInfoLayout = new QVBoxLayout();
	
	topInfoLayout->addWidget(annotationView, 1);
	topInfoLayout->addWidget(rulerView, 0);
	topInfoLayout->addWidget(lcbView, 0);
	topInfoLayout->addWidget(referenceView, 0);
	topInfoLayout->setMargin(0);
	topInfoLayout->setSpacing(3);
	
	QHBoxLayout * overviewLayout = new QHBoxLayout();
	overviewLayout->setMargin(0);
	overviewLayout->setSpacing(3);
	overviewLayout->addWidget(treeOverview, 2);
	overviewLayout->addWidget(blockViewMap, 3);
	
	QWidget * topInfo = new QWidget();
	topInfo->setLayout(topInfoLayout);
	rulerView->setMaximumHeight(15);
	rulerView->setMinimumHeight(15);
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
	splitterTop->setMaximumHeight(80);
	splitterTop->setMinimumHeight(80);
	layout->addWidget(splitterTop);
	layout->addWidget(splitterMain);
	setLayout(layout);
	
	connect(blockViewMain, SIGNAL(positionChanged(int, int, int)), this, SLOT(setPosition(int, int, int)));
	
	FilterControl * filterControl = new FilterControl(this);
	connect(blockViewMain, SIGNAL(windowChanged(int, int)), this, SLOT(setWindow(int, int)));
	connect(filterControl, SIGNAL(filtersChanged()), blockViewMain, SLOT(updateSnpsNeeded()));
	connect(filterControl, SIGNAL(filtersChanged()), blockViewMain, SLOT(updateSnpsNeeded()));
	connect(treeView, SIGNAL(signalNodeHover(const PhylogenyNode *)), this, SLOT(setNode(const PhylogenyNode *)));
	show();
	//filterControl->setParent(this);
	
	OptionButton * filterButton = new OptionButton(filterControl, "Snps");
	QHBoxLayout * buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch();
	buttonLayout->addWidget(filterButton, 0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
//	buttonLayout->setSpacing(0);
	
//	layout->addLayout(buttonLayout);
	layout->setSpacing(3);
	
	treeView->setPhylogenyTree(&tree);
	treeView->setTrackHeights(trackHeights, trackCount);
	treeView->setOrder(&leafIds);
	treeView->setNames(&names);
	treeOverview->setPhylogenyTree(&tree);
	treeOverview->setTrackHeights(trackHeightsOverview, trackCount);
	treeOverview->setOrder(&leafIds);
	treeOverview->setNames(&names);
//	treeView->setNames(&labels);
	
//	nameListView->setOrder(&leafIds);
//	nameListView->setNames(&names);
//	nameListView->setTrackHeights(trackHeights, leafIds.size());
	
	alignmentView->setIdByTrack(&leafIds);
	alignmentView->setAlignment(&alignment);
	alignmentView->setTrackHeights(trackHeights, trackCount);
	
	alignmentView2->setIdByTrack(&leafIds);
	alignmentView2->setAlignment(&alignment);
	alignmentView->setTrackHeights(trackHeights, trackCount);
	
	alignmentView3->setIdByTrack(&leafIds);
	alignmentView3->setAlignment(&alignment);
	alignmentView->setTrackHeights(trackHeights, trackCount);
	
	blockViewMain->setIdByTrack(&leafIds);
	blockViewMain->setAlignment(&alignment);
	blockViewMain->setTrackHeights(trackHeights, trackCount);
	blockViewMap->setIdByTrack(&leafIds);
	blockViewMap->setAlignment(&alignment);
	blockViewMap->setTrackHeights(trackHeightsOverview, trackCount);
	
	rulerView->setAlignment(&alignment);
	filterControl->setAlignment(&alignment);
	
	QTimer * timer = new QTimer(this); // TODO: delete?
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(20);
}

MainWindow::~MainWindow()
{
	delete [] trackHeights;
	delete [] trackHeightsOverview;
}

void MainWindow::setNode(const PhylogenyNode *node)
{
	treeStatus->setPhylogenyNode(node, tr(""));
}

void MainWindow::setPosition(int gapped, int ungapped, int offset)
{
	blockStatus->setPosition(ungapped, offset);
	rulerView->setPosition(gapped, ungapped, offset);
	annotationView->setPosition(gapped, ungapped, offset);
}

void MainWindow::setTrackFocus(int track)
{
	trackFocusLast = trackFocus;
	trackFocus = track;
	trackZoomStartLast = trackZoomStart;
	trackZoomEndLast = trackZoomEnd;
	timerFocus.initialize(325);
	treeView->setTrackFocus(trackFocus);
	alignmentView->setTrackFocus(trackFocus);
	blockViewMain->setTrackFocus(trackFocus);
}

void MainWindow::setTrackHover(int track, int trackEnd)
{
	//if ( trackListViewFocus != (TrackListView *)QObject::sender() ) return;
	treeView->setTrackHover(track, trackEnd);
//	nameListView->setHighlightTrack(track);
	alignmentView->setTrackHover(track, trackEnd);
	blockViewMain->setTrackHover(track, trackEnd);
	
	if ( track == trackEnd )
	{
		if ( track == -1 )
		{
			treeStatus->setPhylogenyNode(0, tr(""), track);
		}
		else
		{
			treeStatus->setPhylogenyNode(tree.getLeaf(track), names[leafIds[track]], track);
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
		timerFocus.initialize( 325);
	}
	
//	blockViewMap->setTrackZoom(start, end);
}

void MainWindow::setWindow(int start, int end)
{
	annotationView->setWindow(start, end);
	rulerView->setWindow(start, end);
	blockViewMap->setWindow(start, end);
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
	bool timerTrackZoomUpdated = timerTrackZoom.update();
	
	if ( timerFocusUpdated || timerTrackZoomUpdated )
	{
		updateTrackHeights();
		
		if ( timerFocusUpdated )
		{
			treeView->setZoomProgress(timerFocus.getProgress());
		}
		
		if ( trackListViewFocus )
		{
//			trackListViewFocus->handleTrackHeightChange();
		}
//		treeView->handleTrackHeightChange(trackListViewFocus);
//		blockViewMain->handleTrackHeightChange(trackListViewFocus);
//		alignmentView->handleTrackHeightChange(trackListViewFocus);
	}
	
	if ( alignmentView->getTweenNeeded() || alignmentView2->getTweenNeeded() || alignmentView3->getTweenNeeded() )
	{
		timerAlignment.initialize(1250);
	}
	
	treeView->update();
	//nameListView->update();
	timerAlignment.update();
	
	alignmentView->update(timerAlignment.getProgress());
//	alignmentView2->update(timerAlignment.getProgress());
//	alignmentView3->update(timerAlignment.getProgress());
	blockViewMain->update();
	blockViewMap->update();
	annotationView->update();
	rulerView->update();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	updateTrackHeights();
	
	float factor = (float)blockViewMap->getHeight() / trackCount;
	
	for ( int i = 0; i < trackCount + 1; i++ )
	{
		trackHeightsOverview[i] = i * factor;
	}
}

void MainWindow::connectTrackListView(TrackListView *view)
{
	connect(view, SIGNAL(signalTrackHoverChange(int, int)), this, SLOT(setTrackHover(int, int)));
	connect(view, SIGNAL(signalTrackFocusChange(int)), this, SLOT(setTrackFocus(int)));
	connect(view, SIGNAL(signalFocus(TrackListView *)), this, SLOT(setTrackListViewFocus(TrackListView *)));
	connect(view, SIGNAL(signalUnfocus(TrackListView *)), this, SLOT(setTrackListViewFocus(TrackListView *)));
}

bool MainWindow::loadXml(const QString & fileName)
{
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
	tree.loadDom(&treeElement);
	
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
	
	trackHeights[0] = 0;
	
	for ( int i = 1; i < trackCount + 1; i++ )
	{
		trackHeights[i] = trackHeights[i - 1] + 1;
		
		int distance;
		
		if ( i <= trackFocus )
		{
			distance = trackFocus - i + 1;
		}
		else
		{
			distance = i - trackFocus - 1;
		}
		
		if ( distance < radius && trackFocus != -1 )
		{
			trackHeights[i] += 5 * timerFocus.getProgress() * cos(distance * 3.1416 / radius / 2);
		}
		
		int distanceLast;
		
		if ( i <= trackFocusLast && trackFocusLast != -1 )
		{
			distanceLast = trackFocusLast - i + 1;
		}
		else
		{
			distanceLast = i - trackFocusLast - 1;
		}
		
		if ( trackFocusLast != -1 && distanceLast < radius )
		{
			trackHeights[i] += 5 * (1 - timerFocus.getProgress()) * cos(distanceLast * 3.1416 / radius / 2);
		}
	}
	
	int viewHeight = treeView->getHeight();
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
	
	for ( int i = 1; i < trackCount + 1; i++ )
	{
		trackHeights[i] = trackHeights[i] * yFactor + yOffset;
	}
	
	treeView->handleTrackHeightChange(trackListViewFocus);
	blockViewMain->handleTrackHeightChange(trackListViewFocus);
	alignmentView->handleTrackHeightChange(trackListViewFocus);
	//	emit heightsChanged();
}
