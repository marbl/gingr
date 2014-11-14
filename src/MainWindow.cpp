// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

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

using namespace::std;

//using namespace google::protobuf::io;

MainWindow::MainWindow(int argc, char ** argv, QWidget * parent)
: QMainWindow(parent)
{
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
	
	QCoreApplication::setOrganizationName("MarBL");
	QCoreApplication::setOrganizationDomain("github.com/marbl");
	QCoreApplication::setApplicationName("Gingr");
	
	inContextMenu = false;
	
	blockViewMain = 0;
	
	setWindowTitle(tr("Gingr"));
	
	resize(1600, 900);
	
	QWidget * widgetCentral = new QWidget(this);
	setCentralWidget(widgetCentral);
	
	//QMenuBar * menuBar = new QMenuBar(this);
	QMenu * menuFile = menuBar()->addMenu("File");
	menuHelp = menuBar()->addMenu("Help");
	
	QAction * actionNew = new QAction(tr("&New workspace..."), this);
	actionNew->setShortcut(QKeySequence("Ctrl+N"));
	menuFile->addAction(actionNew);
	connect(actionNew, SIGNAL(triggered()), this, SLOT(menuNew()));
	
	QAction * actionOpen = new QAction(tr("&Open..."), this);
	actionOpen->setShortcut(QKeySequence("Ctrl+O"));
	menuFile->addAction(actionOpen);
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(menuOpen()));
	
	actionSave = new QAction(tr("&Save workspace..."), this);
	actionSave->setShortcut(QKeySequence("Ctrl+S"));
	actionSave->setDisabled(true);
	menuFile->addAction(actionSave);
	connect(actionSave, SIGNAL(triggered()), this, SLOT(menuSave()));
	
	actionSaveAs = new QAction(tr("Save workspace as..."), this);
//	actionSaveAs->setShortcut(QKeySequence("Ctrl+S"));
	actionSaveAs->setDisabled(true);
	menuFile->addAction(actionSaveAs);
	connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(menuSaveAs()));
	
	QAction * actionSeparator1 = new QAction(this);
	actionSeparator1->setSeparator(true);
	menuFile->addAction(actionSeparator1);
	
	actionExportAlignmentXmfa = new QAction(tr("Export alignment (XMFA)..."), this);
	menuFile->addAction(actionExportAlignmentXmfa);
	actionExportAlignmentXmfa->setDisabled(true);
	connect(actionExportAlignmentXmfa, SIGNAL(triggered()), this, SLOT(menuExportAlignmentXmfa()));
	
	actionExportTree = new QAction(tr("Export tree (Newick)..."), this);
	menuFile->addAction(actionExportTree);
	actionExportTree->setDisabled(true);
	connect(actionExportTree, SIGNAL(triggered()), this, SLOT(menuExportTree()));
	
	actionExportVariantsMfa = new QAction(tr("Export variants (MFA)..."), this);
	menuFile->addAction(actionExportVariantsMfa);
	actionExportVariantsMfa->setDisabled(true);
	connect(actionExportVariantsMfa, SIGNAL(triggered()), this, SLOT(menuExportVariantsMfa()));
	
	actionExportVariantsVcf = new QAction(tr("Export variants (VCF)..."), this);
	menuFile->addAction(actionExportVariantsVcf);
	actionExportVariantsVcf->setDisabled(true);
	connect(actionExportVariantsVcf, SIGNAL(triggered()), this, SLOT(menuExportVariantsVcf()));
	
	QAction * actionSeparator3 = new QAction(this);
	actionSeparator3->setSeparator(true);
	menuFile->addAction(actionSeparator3);
	
	actionExportImage = new QAction(tr("Sna&pshot..."), this);
	actionExportImage->setShortcut(QKeySequence("Ctrl+P"));
	menuFile->addAction(actionExportImage);
	connect(actionExportImage, SIGNAL(triggered()), this, SLOT(menuSnapshot()));
	actionExportImage->setDisabled(true);
	
	QAction * actionHelp = new QAction(tr("Gingr &Help"), this);
	actionHelp->setShortcut(QKeySequence("F1"));
	menuHelp->addAction(actionHelp);
	connect(actionHelp, SIGNAL(triggered()), this, SLOT(menuActionHelp()));
	snapshotWindow = new SnapshotWindow(this);
	connect(snapshotWindow, SIGNAL(signalSnapshot(const QString &, bool, bool)), this, SLOT(saveSnapshot(const QString &, bool, bool)));
	
	connect(this, SIGNAL(signalWarning(const QString &)), this, SLOT(warning(const QString &)));
	
	importWindow = new ImportWindow(this);
	connect(importWindow, SIGNAL(signalOpen(const QString &, const QString &, ImportWindow::FileType)), this, SLOT(import(const QString &, const QString &, ImportWindow::FileType)));
	
	trackCount = 0;
	
	show();
	
	if ( argc > 1 )
	{
		int i = 1;
		while ( i < argc && argv[i][0] == '-' )
		{
			i += 2;
		}
		
		if ( i < argc )
		{
			initializeLayout();
			loadHarvest(QString(argv[i]));
		}
		//loadXml(QString(argv[1]));
	}
	
	if ( ! blockViewMain )
	{
		QHBoxLayout * layout = new QHBoxLayout();
		QTextBrowser * text = new QTextBrowser(this);
		
		text->setSource(QUrl("qrc:/html/splash.html"));
		text->setFrameStyle(QFrame::NoFrame);
		text->setOpenExternalLinks(true);
		
		layout->setMargin(0);
		layout->addWidget(text);
		
		centralWidget()->setLayout(layout);
	}
	
//	actionToggleLightColors->setChecked(true);
}

MainWindow::~MainWindow()
{
	if ( trackHeights )
	{
		delete [] trackHeights;
		delete [] trackHeightsOverview;
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

bool MainWindow::documentChanged()
{
	return actionSave->isEnabled();
}

void MainWindow::menuExportAlignmentXmfa()
{
	QString fileName = QFileDialog::getSaveFileName
	(
	 this,
	 tr("Save XMFA file"),
	 getDefaultDirectory(),
	 tr("XMFA (*.xmfa)")
	 );
	
	if ( ! fileName.isNull() )
	{
		setDefaultDirectoryFromFile(fileName);
		exportFile(fileName, ImportWindow::ALN_XMFA);
	}
}

void MainWindow::menuExportTree()
{
	QString fileName = QFileDialog::getSaveFileName
	(
	 this,
	 tr("Save Newick file"),
	 getDefaultDirectory(),
	 tr("Newick (*.nwk)")
	 );
	
	if ( ! fileName.isNull() )
	{
		setDefaultDirectoryFromFile(fileName);
		exportFile(fileName, ImportWindow::TRE_NWK);
	}
}

void MainWindow::menuExportVariantsMfa()
{
	QString fileName = QFileDialog::getSaveFileName
	(
	 this,
	 tr("Save MFA file"),
	 getDefaultDirectory(),
	 tr("MFA (*.mfa)")
	 );
	
	if ( ! fileName.isNull() )
	{
		setDefaultDirectoryFromFile(fileName);
		exportFile(fileName, ImportWindow::VAR_MFA);
	}
}

void MainWindow::menuExportVariantsVcf()
{
	QString fileName = QFileDialog::getSaveFileName
	(
	 this,
	 tr("Save Vcf file"),
	 getDefaultDirectory(),
	 tr("VCF (*.vcf)")
	 );
	
	if ( ! fileName.isNull() )
	{
		setDefaultDirectoryFromFile(fileName);
		exportFile(fileName, ImportWindow::VAR_VCF);
	}
}

void MainWindow::menuOpen()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open alignment file"), getDefaultDirectory(),
		tr(
		   "All known file types (*.ggr *.hvt *.xmfa *.maf *.nxs *.mfa *.fasta *.fna *.fa *.afa *.aln *.gb *.gbk *.gbf *.tre *.tree *.nwk *.nxs *.nex *.vcf);;"
		   "All files (*);;"
		   "Gingr workspace files (*.ggr *.hvt);;"
		   "Alignments (*.xmfa *.maf *.nxs *.nex *.mfa *.fasta *.fna *.fa *.afa *.aln);;"
		   "Trees (*.tre *.tree *.nwk *.nxs *.nex);;"
		   "Annotations (*.gb *.gbk *.gbf);;"
		   "Sequences (*.fasta *.fna *.fa *.gb *.gbk *.gbf);;"
		   "Nexus (*.nxs *.nex);;"
		   ));
	
	if ( ! fileName.isNull() )
	{
		setDefaultDirectoryFromFile(fileName);
		
		if ( fileName.endsWith(".ggr") || fileName.endsWith(".hvt") )
		{
			if ( blockViewMain )
			{
				clear();
			}
			
			loadHarvest(fileName);
			setDocumentLoaded();
			setDocumentUnchanged();
			harvestFileCurrent = fileName;
		}
		else if
		(
			fileName.endsWith(".nwk") ||
			fileName.endsWith(".tree") ||
			fileName.endsWith(".tre")
		)
		{
			loadTree(fileName);
		}
		else if ( ImportWindow::fileIsGenbank(fileName) )
		{
			loadAnnotations(fileName);
		}
		else if ( fileName.endsWith(".maf") )
		{
			loadAlignment(fileName, "", ImportWindow::ALN_MAF);
		}
		else
		{
			importWindow->show();
			importWindow->setFile(fileName);
		}
	}
}

void MainWindow::import(const QString & fileName, const QString & fileNameReference, ImportWindow::FileType type)
{
	setDefaultDirectoryFromFile(fileName);
	
	switch (type)
	{
		case ImportWindow::GINGR:
			loadHarvest(fileName);
			break;
		case ImportWindow::VAR_VCF:
		case ImportWindow::ALN_MFA:
		case ImportWindow::ALN_MAF:
		case ImportWindow::ALN_XMFA:
		case ImportWindow::ALN_NXS:
		case ImportWindow::NEXUS:
			loadAlignment(fileName, fileNameReference, type);
			break;
		case ImportWindow::TRE_NWK:
		case ImportWindow::TRE_NXS:
			loadTree(fileName);
			break;
		case ImportWindow::ANN_GBK:
			loadAnnotations(fileName);
			break;
		case ImportWindow::VAR_MFA:
		default:
			printf("ERROR: unknown import type\n");
	}
}

void MainWindow::menuNew()
{
	if ( ! promptSave() )
	{
		return;
	}
	
	if ( ! blockViewMain )
	{
		initializeLayout();
	}
	else
	{
		clear();
	}
	
	setDocumentUnchanged();
	setDocumentUnloaded();
	
	harvestFileCurrent.clear();
	setWindowTitle(tr("Gingr - Untitled"));
}

bool MainWindow::menuSave()
{
	if ( harvestFileCurrent.length() )
	{
		writeHarvest();
		return true;
	}
	else
	{
		return menuSaveAs();
	}
}

bool MainWindow::menuSaveAs()
{
	QString fileName = QFileDialog::getSaveFileName
	(
		this,
		tr("Save Gingr file"),
		getDefaultDirectory(),
		tr("Gingr (*.ggr)")
	);
	
	if ( ! fileName.isNull() )
	{
		setDefaultDirectoryFromFile(fileName);
		harvestFileCurrent = fileName;
		writeHarvest();
		return true;
	}
	
	return false;
}

void MainWindow::menuSnapshot()
{
	snapshotWindow->show();
}

bool MainWindow::promptSave()
{
	if ( documentChanged() )
	{
		QMessageBox msgBox;
		msgBox.setText("Save changes?");
		//msgBox.setInformativeText("Do you want to save your changes?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
		
		if ( ret == QMessageBox::Save )
		{
			return menuSave();
		}
		else
		{
			return ret == QMessageBox::Discard;
		}
	}
	
	return true;
}

void MainWindow::rerootTree(const PhylogenyTreeNode * rootNew)
{
	int rowFocus = -1;
	
	if ( trackFocus != -1 )
	{
		rowFocus = leafIds[trackFocus];
	}
	
	if ( rootNew )
	{
		hio.phylogenyTree.setOutgroup(rootNew);
	}
	else
	{
		hio.phylogenyTree.midpointReroot();
	}
	
	hio.phylogenyTree.getLeafIds(leafIds);
	
	if ( rowFocus != -1 )
	{
		for ( int i = 0; i < leafIds.size(); i++ )
		{
			if ( leafIds[i] == rowFocus )
			{
				trackFocus = i;
				break;
			}
		}
	}
	
	treeViewMain->setPhylogenyTree(&hio.phylogenyTree);
	treeViewMap->setPhylogenyTree(&hio.phylogenyTree);
	
	treeViewMain->setTrackFocus(trackFocus);
	blockViewMain->setTrackFocus(trackFocus);
	
	updateTrackHeights();
	
	treeViewMain->setNames(&names);
	treeViewMap->setNames(&names);
	
	treeViewMain->setIdByTrack(&leafIds);
	treeViewMap->setIdByTrack(&leafIds);
	blockViewMain->setIdByTrack(&leafIds);
	blockViewMap->setIdByTrack(&leafIds);
	
	if ( alignment.getTrackReference() != -1 )
	{
		treeViewMain->setTrackReference(alignment.getTrackReference());
	}
	
	blockViewMap->setBufferUpdateNeeded();
	blockViewMain->setBufferUpdateNeeded();
	
	setTrackZoom(0, leafIds.size() - 1);
	
	setDocumentChanged();
}

void MainWindow::rerootTreeMidpoint()
{
	rerootTree();
}

void MainWindow::setInContextMenu(bool inContextMenuNew)
{
	inContextMenu = inContextMenuNew;
}

void MainWindow::toggleShowGaps(bool checked)
{
	if ( checked )
	{
		showGaps |= Alignment::SHOW;
		actionToggleShowInsertions->setEnabled(true);
		actionToggleShowDeletions->setEnabled(true);
	}
	else
	{
		showGaps &= ~Alignment::SHOW;
		actionToggleShowInsertions->setEnabled(false);
		actionToggleShowDeletions->setEnabled(false);
	}
	
	blockStatus->setShowGaps(checked);
	
	updateSnpsMain();
	updateSnpsMap();
}

void MainWindow::toggleShowInsertions(bool checked)
{
	if ( checked )
	{
		showGaps |= Alignment::INSERTIONS;
	}
	else
	{
		showGaps &= ~Alignment::INSERTIONS;
	}
	
	blockStatus->setShowIns(checked);
	
	updateSnpsMain();
	updateSnpsMap();
}

void MainWindow::toggleShowDeletions(bool checked)
{
	if ( checked )
	{
		showGaps |= Alignment::DELETIONS;
	}
	else
	{
		showGaps &= ~Alignment::DELETIONS;
	}
	
	blockStatus->setShowDel(checked);
	
	updateSnpsMain();
	updateSnpsMap();
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
	//blockViewMain->setLightColors(lightColors);
	//blockViewMap->setLightColors(lightColors);
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
	if ( alignment.getLength() == 0 )
	{
		return; // TODO: only tree loaded?
	}
	
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

void MainWindow::setDocumentChanged()
{
	actionSave->setDisabled(false);
	actionSaveAs->setDisabled(false);
}

void MainWindow::setDocumentLoaded()
{
	actionSaveAs->setDisabled(false);
	
}
void MainWindow::setDocumentUnchanged()
{
	actionSave->setDisabled(true);
}

void MainWindow::setDocumentUnloaded()
{
	actionSaveAs->setDisabled(true);
}

void MainWindow::setNode(const PhylogenyTreeNode *node)
{
	if (node->getChildrenCount() )
	{
		treeStatus->setPhylogenyTreeNode(node, tr(""));
	}
	else
	{
		treeStatus->setPhylogenyTreeNode(node, names[leafIds[node->getLeafMin()]]);
	}
}

void MainWindow::setPosition(int gapped)
{
	if ( inContextMenu || ! alignment.getLength() )
	{
		return;
	}
	
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
	if ( inContextMenu || loading )
	{
		return;
	}
	
	//if ( trackListViewFocus != (TrackListView *)QObject::sender() ) return;
	treeViewMain->setTrackHover(track, trackEnd);
//	nameListView->setHighlightTrack(track);
//	alignmentView->setTrackHover(track, trackEnd);
	blockViewMain->setTrackHover(track, trackEnd);
	
	if ( track == trackEnd )
	{
		if ( track == -1 )
		{
			treeStatus->setPhylogenyTreeNode(0, tr(""), track);
		}
		else
		{
			if ( hio.phylogenyTree.getRoot() )
			{
				treeStatus->setPhylogenyTreeNode(hio.phylogenyTree.getLeaf(track), names[leafIds[track]], track);
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

void MainWindow::setTrackReference(int trackReferenceNew)
{
	alignment.setTrackReference(trackReferenceNew);
	treeViewMain->setTrackReference(trackReferenceNew);
	hio.trackList.setTrackReference(trackReferenceNew);
	updateSnpsMain();
	updateSnpsMap();
	setDocumentChanged();
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
		timerFocus.initialize(350);
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
		
		if ( timerFocus.getProgress() == 1 )
		{
			updateSnpsMain();
			//updateSnpsMap();
		}
		
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
	snpBufferMain.update(posStart * 2 - posEnd - 1, 2 * posEnd - posStart + 1, 3 * blockViewMain->getWidth(), trackZoomStart, trackZoomEnd, synteny, lightColors, showGaps);
}

void MainWindow::updateSnpsMap()
{
	snpBufferMap.update(-alignment.getLength(), 2 * alignment.getLength() - 1, 3 * blockViewMap->getWidth(), 0, trackCount - 1, synteny, lightColors, showGaps);
}

void MainWindow::warning(const QString & message)
{
	QMessageBox::warning(this, tr("Warning"), message);
}

void MainWindow::zoomFromMouseWheel(int delta)
{
	if ( ! alignment.getLength() )
	{
		return;
	}
	
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
	int left = qRound(float(posCursor - posStart) * size / sizeCur);
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

void MainWindow::clear()
{
	blockViewMain->clear();
	blockViewMap->clear();
	treeViewMain->clear();
	treeViewMap->clear();
	referenceView->clear();
	annotationView->clear();
	rulerView->clear();
	blockStatus->clear();
	
	trackFocus = -1;
	trackFocusLast = -1;
	
	if ( trackHeights )
	{
		delete [] trackHeights;
		delete [] trackHeightsOverview;
		trackHeights = 0;
	}
	
	hio.clear();
	
	blockStatus->setShowLegend(false);
	
	//actionImportAnnotations->setDisabled(true);
	actionExportAlignmentXmfa->setDisabled(true);
	actionExportImage->setDisabled(true);
	actionExportTree->setDisabled(true);
	actionExportVariantsMfa->setDisabled(true);
	actionExportVariantsVcf->setDisabled(true);
	actionMidpointReroot->setDisabled(true);
}

void MainWindow::clearAlignment()
{
	blockViewMain->clear();
	blockViewMap->clear();
	referenceView->clear();
	annotationView->clear();
	rulerView->clear();
	
	actionExportAlignmentXmfa->setDisabled(true);
	actionExportVariantsMfa->setDisabled(true);
	actionExportVariantsVcf->setDisabled(true);
	
	if ( trackHeights )
	{
		delete [] trackHeights;
		delete [] trackHeightsOverview;
		trackHeights = 0;
	}
	
	hio.referenceList.clear();
	hio.annotationList.clear();
	hio.lcbList.clear();
	hio.variantList.clear();
}

void MainWindow::clearTree()
{
	hio.phylogenyTree.clear();
	treeViewMain->clear();
	treeViewMap->clear();
	actionExportTree->setDisabled(true);
}

void MainWindow::connectTrackListView(TrackListView *view)
{
	connect(view, SIGNAL(signalTrackHoverChange(int, int)), this, SLOT(setTrackHover(int, int)));
	connect(view, SIGNAL(signalTrackFocusChange(int)), this, SLOT(setTrackFocus(int)));
	connect(view, SIGNAL(signalFocus(TrackListView *)), this, SLOT(setTrackListViewFocus(TrackListView *)));
	connect(view, SIGNAL(signalUnfocus(TrackListView *)), this, SLOT(setTrackListViewFocus(TrackListView *)));
}

void MainWindow::exportFile(const QString &fileName, ImportWindow::FileType type)
{
	QFileInfo fileInfo(fileName);
	QProgressDialog dialog;
	dialog.setCancelButton(0);
	dialog.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	dialog.setLabelText(QString("Saving %1...").arg(fileInfo.fileName()));
	// Create a QFutureWatcher and connect signals and slots.
	QFutureWatcher<void> futureWatcher;
	QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
	//	QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
	QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
	//	QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
	
	// Start the computation.
	futureWatcher.setFuture(QtConcurrent::run(this, &MainWindow::exportFileBackground, fileName, type));
	
	inContextMenu = true;
	
	// Display the dialog and start the event loop.
	dialog.exec();
	
	futureWatcher.waitForFinished();
	
	if ( futureWatcher.isCanceled() )
	{
		names.resize(0);
		labels.resize(0);
		return;
	}
	
	inContextMenu = false;
}

void MainWindow::exportFileBackground(const QString & fileName, ImportWindow::FileType type)
{
	std::ofstream out;
	out.open(fileName.toStdString().c_str());
	
	switch (type)
	{
		case ImportWindow::ALN_XMFA:
			hio.writeXmfa(out);
			break;
		case ImportWindow::TRE_NWK:
			hio.writeNewick(out);
			break;
		case ImportWindow::VAR_MFA:
			hio.writeSnp(out);
			break;
		case ImportWindow::VAR_VCF:
			hio.writeVcf(out);
			break;
	}
}

const QString MainWindow::getDefaultDirectory()
{
    QSettings settings;
	return settings.value(DEFAULT_DIR_KEY).toString();
}

void MainWindow::initialize()
{
	if ( true || hio.phylogenyTree.getRoot() )
	{
		initializeTree();
	}
	
	if ( hio.lcbList.getLcbCount() )
	{
		initializeAlignment();
	}
	
/*	annotationView->setAlignment(&alignment);
	
	snpBufferMain.initialize(&alignment);
	snpBufferMap.initialize(&alignment);
	filterControl->setAlignment(&alignment);
	
	hio.phylogenyTree.getLeafIds(leafIds);
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
	treeViewMain->setPhylogenyTree(&hio.phylogenyTree);
	treeViewMain->setNames(&names);
	treeViewMain->setTrackReference(alignment.getTrackReference());
	
	treeViewMap->setTrackHeights(trackHeightsOverview, trackCount);
	treeViewMap->setIdByTrack(&leafIds);
	treeViewMap->setPhylogenyTree(&hio.phylogenyTree);
	treeViewMap->setNames(&names);
	//	treeViewMain->setNames(&labels);
	
	//	nameListView->setOrder(&leafIds);
	//	nameListView->setNames(&names);
	//	nameListView->setTrackHeights(trackHeights, leafIds.size());
	//lcbView->setAlignment(&alignment);
	rulerView->setAlignment(&alignment);
	referenceView->setAlignment(&alignment);
	referenceView->setSnpBuffer(&snpBufferMain);
	
	leafDists = new float[trackCount];
	leafDists[0] = 0;
	float maxDist = 0;
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
	blockStatus->setCore(alignment.getCore());*/
}

void MainWindow::initializeAlignment()
{
	setDocumentChanged();
	
	annotationView->setAlignment(&alignment);
	
	snpBufferMain.initialize(&alignment, &leafIds, &mutexAlignment);
	snpBufferMap.initialize(&alignment, &leafIds, &mutexAlignment);
	
	//tree.getLeafIds(leafIds);
	
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
	blockViewMain->setSnpBuffer(&snpBufferMain);
	blockViewMain->setIdByTrack(&leafIds);
	blockViewMain->setTrackHeights(trackHeights, trackCount);
	blockViewMain->setAlignment(&alignment);
	//blockViewMap->setLeafDists(leafDists);
	blockViewMap->setSnpBuffer(&snpBufferMap);
	blockViewMap->setIdByTrack(&leafIds);
	blockViewMap->setTrackHeights(trackHeightsOverview, trackCount);
	blockViewMap->setAlignment(&alignment);
	
	filterControl->setAlignment(&alignment);
	searchControl->initialize();
	
	updateTrackHeightsOverview();
	//updateSnpsMain();
	updateSnpsMap();
	
	blockStatus->setShowLegend(true);
	blockStatus->setTracks(trackCount);
	blockStatus->setCore(alignment.getCore());
	
	actionExportAlignmentXmfa->setDisabled(false);
	actionExportImage->setDisabled(false);
	actionExportVariantsMfa->setDisabled(false);
	actionExportVariantsVcf->setDisabled(false);
}

void MainWindow::initializeLayout()
{
	trackFocus = -1;
	trackFocusLast = -1;
	trackListViewFocus = 0;
	trackCount = 0;
	synteny = false;
	trackHeights = 0;
	trackHeightsOverview = 0;
	lightColors = true;
	zoom = 1;
	help = 0;
	
	showGaps = Alignment::INSERTIONS | Alignment::DELETIONS;
	
	bool showIns = showGaps & Alignment::INSERTIONS;
	bool showDel = showGaps & Alignment::DELETIONS;
	
	QMenu * menuTree = menuBar()->addMenu("Tree");
	QMenu * menuView = menuBar()->addMenu("View");
	QMenu * menuWindow = menuBar()->addMenu("Window");
	
	menuBar()->insertMenu(menuHelp->menuAction(), menuTree);
	menuBar()->insertMenu(menuHelp->menuAction(), menuView);
	menuBar()->insertMenu(menuHelp->menuAction(), menuWindow);
	
	actionMidpointReroot = new QAction(tr("Reroot at midpoint"), this);
	menuTree->addAction(actionMidpointReroot);
	actionMidpointReroot->setDisabled(true);
	connect(actionMidpointReroot, SIGNAL(triggered()), this, SLOT(rerootTreeMidpoint()));
	
	actionToggleSynteny = new QAction(tr("Synten&y"), this);
	actionToggleSynteny->setCheckable(true);
	actionToggleSynteny->setShortcut(QKeySequence("Ctrl+Y"));
	menuView->addAction(actionToggleSynteny);
	connect(actionToggleSynteny, SIGNAL(triggered()), this, SLOT(toggleSynteny()));
	
	QAction * actionSeparator3 = new QAction(this);
	actionSeparator3->setSeparator(true);
	menuView->addAction(actionSeparator3);
	
	/*
	QAction * actionRightAlignNodes = new QAction(tr("&Right-align clades"), this);
	actionRightAlignNodes->setCheckable(true);
	actionRightAlignNodes->setShortcut(QKeySequence("Ctrl+R"));
	menuView->addAction(actionRightAlignNodes);
	connect(actionRightAlignNodes, SIGNAL(toggled(bool)), this, SLOT(toggleRightAlignNodes(bool)));
	*/
	
	QAction * actionRightAlignText = new QAction(tr("&Right-align labels"), this);
	actionRightAlignText->setShortcut(QKeySequence("Ctrl+R"));
	actionRightAlignText->setCheckable(true);
	menuView->addAction(actionRightAlignText);
	connect(actionRightAlignText, SIGNAL(toggled(bool)), this, SLOT(toggleRightAlignText(bool)));
	
	QAction * actionSeparator4 = new QAction(this);
	actionSeparator4->setSeparator(true);
	menuView->addAction(actionSeparator4);
	
	/*
	QAction * actionToggleShowGaps = new QAction(tr("Highlight &gaps"), this);
	actionToggleShowGaps->setShortcut(QKeySequence("Ctrl+G"));
	actionToggleShowGaps->setCheckable(true);
	actionToggleShowGaps->setChecked(showGaps & Alignment::SHOW);
	menuView->addAction(actionToggleShowGaps);
	connect(actionToggleShowGaps, SIGNAL(toggled(bool)), this, SLOT(toggleShowGaps(bool)));
	
	actionToggleShowInsertions = new QAction(tr("...for &insertions"), this);
	actionToggleShowInsertions->setShortcut(QKeySequence("Ctrl+I"));
	actionToggleShowInsertions->setCheckable(true);
	actionToggleShowInsertions->setChecked(showIns);
	actionToggleShowInsertions->setEnabled(showGaps & Alignment::SHOW);
	menuView->addAction(actionToggleShowInsertions);
	connect(actionToggleShowInsertions, SIGNAL(toggled(bool)), this, SLOT(toggleShowInsertions(bool)));
	
	actionToggleShowDeletions = new QAction(tr("...for &deletions"), this);
	actionToggleShowDeletions->setShortcut(QKeySequence("Ctrl+D"));
	actionToggleShowDeletions->setCheckable(true);
	actionToggleShowDeletions->setChecked(showDel);
	actionToggleShowDeletions->setEnabled(showGaps & Alignment::SHOW);
	menuView->addAction(actionToggleShowDeletions);
	connect(actionToggleShowDeletions, SIGNAL(toggled(bool)), this, SLOT(toggleShowDeletions(bool)));
	
	
	QAction * actionSeparator5 = new QAction(this);
	actionSeparator5->setSeparator(true);
	menuView->addAction(actionSeparator5);
	*/
	
	QAction * actionToggleLightColors = new QAction(tr("&Light colors"), this);
	actionToggleLightColors->setShortcut(QKeySequence("Ctrl+L"));
	actionToggleLightColors->setCheckable(true);
	actionToggleLightColors->setChecked(lightColors);
	menuView->addAction(actionToggleLightColors);
	connect(actionToggleLightColors, SIGNAL(toggled(bool)), this, SLOT(toggleLightColors(bool)));
	
	actionSnps = new QAction(tr("&Variants"), this);
	actionSnps->setShortcut(QKeySequence("Ctrl+V"));
	actionSnps->setCheckable(true);
	menuWindow->addAction(actionSnps);
	connect(actionSnps, SIGNAL(toggled(bool)), this, SLOT(toggleSnps(bool)));
	
	actionSearch = new QAction(tr("&Find"), this);
	actionSearch->setShortcut(QKeySequence("Ctrl+F"));
	actionSearch->setCheckable(true);
	menuWindow->addAction(actionSearch);
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
	connect(treeViewMain, SIGNAL(signalFocusNode(const PhylogenyTreeNode *, bool)), treeViewMap, SLOT(setFocusNode(const PhylogenyTreeNode *, bool)));
	connect(treeViewMain, SIGNAL(signalReroot(const PhylogenyTreeNode *)), this, SLOT(rerootTree(const PhylogenyTreeNode *)));
	connect(treeViewMain, SIGNAL(signalContextMenu(bool)), this, SLOT(setInContextMenu(bool)));
	
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
	blockStatus->setShowGaps(showGaps & Alignment::SHOW);
	blockStatus->setShowIns(showIns);
	blockStatus->setShowDel(showDel);
	blockStatus->setLightColors(lightColors);
	
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
	
	annotationView->setMinimumWidth(blockStatus->minimumSizeHint().width());
	
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
	overview->setMinimumWidth(treeStatus->minimumSizeHint().width());
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
//	centralWidget()->setLayout(layout);
	
	filterControl = new FilterControl(this);
	searchControl = new SearchControl(this);
	
	connect(blockViewMain, SIGNAL(positionChanged(int)), this, SLOT(setPosition(int)));
	connect(blockViewMain, SIGNAL(windowChanged(int, int)), this, SLOT(setWindow(int, int)));
	connect(blockViewMain, SIGNAL(signalMouseWheel(int)), this, SLOT(zoomFromMouseWheel(int)));
	connect(blockViewMain, SIGNAL(signalContextMenu(bool)), this, SLOT(setInContextMenu(bool)));
	connect(blockViewMain, SIGNAL(signalTrackReference(int)), this, SLOT(setTrackReference(int)));
	
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
	connect(treeViewMain, SIGNAL(signalNodeHover(const PhylogenyTreeNode *)), this, SLOT(setNode(const PhylogenyTreeNode *)));
	connect(filterControl, SIGNAL(closed()), this, SLOT(closeSnps()));
	connect(searchControl, SIGNAL(closed()), this, SLOT(closeSearch()));
	connect(searchControl, SIGNAL(signalSearchChanged(const QString &, bool)), treeViewMain, SLOT(search(const QString &, bool)));
	connect(searchControl, SIGNAL(signalSearchChanged(const QString &, bool)), annotationView, SLOT(search(const QString &, bool)));
	connect(treeViewMain, SIGNAL(signalSearchResults(int)), searchControl, SLOT(resultsChangedTracks(int)));
	connect(annotationView, SIGNAL(signalSearchResults(int)), searchControl, SLOT(resultsChangedAnnotations(int)));
	connect(&snpBufferMain, SIGNAL(updated()), this, SLOT(updateSnpsFinishedMain()));
	connect(&snpBufferMain, SIGNAL(updated()), blockViewMain, SLOT(updateSnpsFinished()));
	connect(&snpBufferMain, SIGNAL(updated()), referenceView, SLOT(updateSnpsFinished()));
	connect(&snpBufferMap, SIGNAL(updated()), blockViewMap, SLOT(updateSnpsFinished()));
	connect(blockViewMain, SIGNAL(signalUpdateSnps()), this, SLOT(updateSnpsMain()));
	connect(blockViewMap, SIGNAL(signalUpdateSnps()), this, SLOT(updateSnpsMap()));
	connect(blockViewMain, SIGNAL(signalToggleSynteny()), this, SLOT(toggleSynteny()));
//	connect(blockStatus->getLabelGaps(), SIGNAL(clicked()), actionToggleShowGaps, SLOT(toggle()));
//	connect(blockStatus->getLabelIns(), SIGNAL(clicked()), actionToggleShowInsertions, SLOT(toggle()));
//	connect(blockStatus->getLabelDel(), SIGNAL(clicked()), actionToggleShowDeletions, SLOT(toggle()));
	
	//filterControl->setParent(this);
	
	//	OptionButton * filterButton = new OptionButton(filterControl, "Snps");
	QHBoxLayout * buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch();
	//	buttonLayout->addWidget(filterButton, 0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	//	buttonLayout->setSpacing(0);
	
	//	layout->addLayout(buttonLayout);
	layout->setSpacing(3);
	
	qDeleteAll(centralWidget()->children());
	delete centralWidget()->layout();
	centralWidget()->setLayout(layout);
	
	setAutoFillBackground(false);
	
	QTimer * timer = new QTimer(this); // TODO: delete?
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(20);
	QApplication::processEvents();
}

void MainWindow::initializeTree()
{
	setDocumentChanged();
	
	trackCount = hio.trackList.getTrackCount();
	
	if ( hio.phylogenyTree.getRoot() )
	{
		leafIds.resize(0);
		hio.phylogenyTree.getLeafIds(leafIds);
		treeViewMain->setPhylogenyTree(&hio.phylogenyTree);
		treeViewMap->setPhylogenyTree(&hio.phylogenyTree);
	}
	else
	{
		leafIds.resize(trackCount);
		
		for ( int i = 0; i < trackCount; i++ )
		{
			leafIds[i] = i;
		}
	}
	
	searchControl->initialize();
	
	if ( ! trackHeights )
	{
		trackCount = leafIds.size();
		trackHeights = new float[leafIds.size() + 1];
		trackHeightsOverview = new float[leafIds.size() + 1];
	}
	
	trackZoomStart = 0;
	trackZoomEnd = trackCount - 1;
	trackZoomStartLast = 0;
	trackZoomEndLast = trackZoomEnd;
	tweenYFactor.initialize(0, 0);
	tweenYOffset.initialize(0, 0);
	timerFocus.initialize(0);
	setTrackZoom(trackZoomStart, trackZoomEnd);
	
	updateTrackHeightsOverview();
	
	treeViewMain->setTrackHeights(trackHeights, trackCount);
	treeViewMain->setIdByTrack(&leafIds);
	treeViewMain->setNames(&names);
	
	treeViewMap->setTrackHeights(trackHeightsOverview, trackCount);
	treeViewMap->setIdByTrack(&leafIds);
	treeViewMap->setNames(&names);
	
	treeViewMain->setTrackReference(alignment.getTrackReference());
	treeViewMap->setTrackReference(alignment.getTrackReference());
	
	actionExportTree->setDisabled(hio.phylogenyTree.getRoot() == 0);
	actionExportImage->setDisabled(false);
	actionMidpointReroot->setDisabled(false);
	
	blockViewMain->setIdByTrack(&leafIds);
	blockViewMap->setIdByTrack(&leafIds);
}

void MainWindow::loadAlignment(const QString &fileName, const QString &fileNameRef, ImportWindow::FileType type)
{
	if ( ! blockViewMain )
	{
		initializeLayout();
	}
	
	clearAlignment();
	
	if ( ! hio.phylogenyTree.getRoot() )
	{
		treeViewMain->clear();
		treeViewMap->clear();
	}
	
	if ( false && async )
	{
		inContextMenu = true;
		QFileInfo fileInfo(fileName);
		QProgressDialog dialog;
		dialog.setCancelButton(0);
		dialog.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
		dialog.setLabelText(QString("Importing %1...").arg(fileInfo.fileName()));
		QFutureWatcher<void> futureWatcher;
		QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
		QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
		futureWatcher.setFuture(QtConcurrent::run(this, &MainWindow::loadAlignmentBackground, fileName, fileNameRef, type));
		
		//printf("%s\n", fileName.toStdString().c_str());
		
		// Display the dialog and start the event loop.
		dialog.exec();
		
		futureWatcher.waitForFinished();
		inContextMenu = false;
	}
	else
	{
		loadAlignmentBackground(fileName, fileNameRef, type);
	}
	
	if ( hio.lcbList.getLcbCount() )
	{
		initializeTree();
		initializeAlignment();
		
		if ( hio.annotationList.getAnnotationCount() )
		{
			annotationView->load(hio.annotationList, &alignment);
			annotationView->setWindow(posStart, posEnd);
			//actionImportAnnotations->setEnabled(false);
		}
		else if ( type == ImportWindow::ALN_MFA || type == ImportWindow::VAR_VCF || (type == ImportWindow::ALN_XMFA && ! fileNameRef.isEmpty()) )
		{
			//actionImportAnnotations->setEnabled(true);
		}
	}
}

void MainWindow::loadAlignmentBackground(const QString &fileName, const QString &fileNameRef, ImportWindow::FileType type)
{
	//printf("%s\n", fileName.toStdString().c_str());
	
	while ( true )
	{
		try
		{
			if ( ! fileNameRef.isEmpty() )
			{
				if ( ImportWindow::fileIsGenbank(fileNameRef) )
				{
					hio.loadGenbank(fileNameRef.toStdString().c_str(), true);
				}
				else
				{
					hio.loadFasta(fileNameRef.toStdString().c_str());
				}
			}
			
			switch (type)
			{
				case ImportWindow::ALN_MAF:
					hio.loadMaf(fileName.toStdString().c_str(), true, 0);
					break;
				case ImportWindow::ALN_MFA:
					hio.loadMfa(fileName.toStdString().c_str(), true);
					break;
				case ImportWindow::VAR_VCF:
					hio.loadVcf(fileName.toStdString().c_str());
					break;
				case ImportWindow::ALN_XMFA:
					hio.loadXmfa(fileName.toStdString().c_str(), ! fileNameRef.isEmpty());
			}
			
			loadNames(hio.trackList);
			alignment.init(hio.lcbList, hio.variantList, hio.referenceList, hio.trackList);
			break;
		}
		catch ( const TrackList::TrackNotFoundException & e )
		{
			QMessageBox msgBox;
			
			msgBox.setText("Clear tree?");
			msgBox.setInformativeText(QString(tr("The current tree does not have a leaf named \"%1\".")).arg(QString::fromStdString(e.name)));
			msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Ok);
			
			int ret = msgBox.exec();
			
			if ( ret == QMessageBox::Ok )
			{
				hio.referenceList.clear();
				clearTree();
			}
			else
			{
				break;
			}
		}
		catch ( const AnnotationList::NoSequenceException & e )
		{
			emit signalWarning(QString(tr("Genbank (%1) does not contain sequence")).arg(QString::fromStdString(e.file)));
			break;
		}
		catch ( const VariantList::CompoundVariantException & e )
		{
			emit signalWarning(QString(tr("Indel allele does not contain flanking reference base (line %1)")).arg(e.line));
			break;
		}
		catch (const LcbList::NoCoreException & e )
		{
			emit signalWarning(QString(tr("No alignments involving all %1 sequences")).arg(e.queryCount));
			break;
		}
	}
}

void MainWindow::loadAnnotations(const QString &fileName)
{
	try
	{
		hio.loadGenbank(fileName.toStdString().c_str(), false);
		annotationView->load(hio.annotationList, &alignment);
		annotationView->setWindow(posStart, posEnd);
		
		setDocumentChanged();
	}
	catch ( const AnnotationList::NoGiException & e )
	{
		emit signalWarning(QString(tr("Genbank file \"%1\" does not have GI number; cannot use for annotations unless loaded as reference")).arg(QString::fromStdString(e.file)));
	}
	catch ( const ReferenceList::GiNotFoundException & e )
	{
		emit signalWarning(QString(tr("Could not find with reference with GI \"%1\"")).arg(QString::fromStdString(e.gi)));
	}
}

bool MainWindow::loadHarvest(const QString & fileName)
{
	if ( ! blockViewMain )
	{
		initializeLayout();
	}
	
	QFileInfo fileInfo(fileName);
	
	if ( async )
	{
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
		futureWatcher.setFuture(QtConcurrent::run(this, &MainWindow::loadHarvestBackground, fileName));
		
		inContextMenu = true;
		
		// Display the dialog and start the event loop.
		dialog.exec();
		
		futureWatcher.waitForFinished();
		
		if ( futureWatcher.isCanceled() )
		{
			names.resize(0);
			labels.resize(0);
			return false;
		}
		
		if ( hio.trackList.getTrackCount() )
		{
			initialize();
		}
		
		inContextMenu = false;
	}
	else
	{
		loadHarvestBackground(fileName);
		initialize();
	}
	
	setWindowTitle(tr("Gingr - ").append(fileInfo.fileName()));
	return true;
}

void MainWindow::loadHarvestBackground(const QString &fileName)
{
	if ( ! hio.loadHarvest(fileName.toStdString().c_str()) )
	{
		emit signalWarning(QString(tr("Could not read \"%1\"")).arg(fileName));
	}
	
	loadNames(hio.trackList);
	
	//phylogenyTree = new PhylogenyTree();
	//phylogenyTree->loadPb(hio.harvest.tree());
	
	alignment.init(hio.lcbList, hio.variantList, hio.referenceList, hio.trackList);
	//actionImportAnnotations->setEnabled(true);
	
	if ( hio.annotationList.getAnnotationCount() )
	{
		annotationView->load(hio.annotationList, &alignment);
	}
}

bool MainWindow::loadNames(const TrackList & trackList)
{
	names.resize(trackList.getTrackCount());
	labels.resize(trackList.getTrackCount());
	
	for ( int i = 0; i < trackList.getTrackCount(); i++ )
	{
		const TrackList::Track & track = trackList.getTrack(i);
		
		if ( track.name.length() )
		{
			labels[i] = QString::fromStdString(track.name);
		}
		
		names[i] = QString::fromStdString(track.file);
	}
	
	return true;
}

void MainWindow::loadTree(const QString & fileName)
{
	if ( ! blockViewMain )
	{
		initializeLayout();
	}
	
	treeViewMain->clear();
	treeViewMap->clear();
	setTrackFocus(-1);
	
	try
	{
		hio.loadNewick(fileName.toStdString().c_str());
		
		loadNames(hio.trackList);
		initializeTree();
		blockViewMain->setBufferUpdateNeeded();
		blockViewMap->setBufferUpdateNeeded();
	}
	catch ( const TrackList::TrackNotFoundException & e )
	{
		QMessageBox msgBox;
		
		msgBox.setText("Clear alignment?");
		msgBox.setInformativeText(QString(tr("The alignment does not have a track named \"%1\".")).arg(QString::fromStdString(e.name)));
		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);
		
		int ret = msgBox.exec();
		
		if ( ret == QMessageBox::Ok )
		{
			clearAlignment();
			hio.loadNewick(fileName.toStdString().c_str());
			loadNames(hio.trackList);
		}
		
		initializeTree();
	}
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

void MainWindow::setDefaultDirectoryFromFile(const QString &file)
{
	QSettings settings;
	QDir CurrentDir;
	settings.setValue(DEFAULT_DIR_KEY, CurrentDir.absoluteFilePath(file));
//	settings.sync();
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
//	blockViewMap->update();
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

void MainWindow::writeHarvest()
{
	QFileInfo fileInfo(harvestFileCurrent);
	QProgressDialog dialog;
	dialog.setCancelButton(0);
	dialog.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	dialog.setLabelText(QString("Saving %1...").arg(fileInfo.fileName()));
	// Create a QFutureWatcher and connect signals and slots.
	QFutureWatcher<void> futureWatcher;
	QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
	//	QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
	QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
	//	QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
	
	// Start the computation.
	futureWatcher.setFuture(QtConcurrent::run(this, &MainWindow::writeHarvestBackground));
	
	inContextMenu = true;
	
	// Display the dialog and start the event loop.
	dialog.exec();
	
	futureWatcher.waitForFinished();
	
	if ( futureWatcher.isCanceled() )
	{
		names.resize(0);
		labels.resize(0);
		return;
	}
	
	inContextMenu = false;
	setWindowTitle(tr("Gingr - ").append(fileInfo.fileName()));
	setDocumentUnchanged();
	setDocumentLoaded();
}

void MainWindow::writeHarvestBackground()
{
	hio.writeHarvest(harvestFileCurrent.toStdString().c_str());
}
