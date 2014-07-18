// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "SnapshotWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>

SnapshotWindow::SnapshotWindow(QWidget * parent)
: QDialog(parent)
{
//	setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	setWindowModality(Qt::WindowModal);
	
	QVBoxLayout * layout = new QVBoxLayout();
	
	QLabel * label = new QLabel(tr("Snapshot"));
	checkBoxTree = new QCheckBox(tr("Tree"));
	checkBoxAlignment = new QCheckBox(tr("Alignment"));
	
	connect(checkBoxTree, SIGNAL(clicked()), this, SLOT(update()));
	connect(checkBoxAlignment, SIGNAL(clicked()), this, SLOT(update()));
	
	layout->addWidget(label);
	layout->addWidget(checkBoxTree);
	layout->addWidget(checkBoxAlignment);
	
	QHBoxLayout * layoutButtons = new QHBoxLayout();
	
	QPushButton * buttonCancel = new QPushButton(tr("Cancel"));
	buttonSave = new QPushButton(tr("Save"));
	
	buttonSave->setDefault(true);
	buttonSave->setEnabled(false);
	
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(buttonSave, SIGNAL(clicked()), this, SLOT(save()));
	
	layoutButtons->addWidget(buttonCancel);
	layoutButtons->addWidget(buttonSave);
	
	layout->addLayout(layoutButtons);
	
	setLayout(layout);
	
	setWindowTitle(tr("Snapshot"));
}

void SnapshotWindow::cancel()
{
	hide();
}

void SnapshotWindow::save()
{
	hide();
	
	QString fileName = QFileDialog::getSaveFileName
	(
	 this,
	 tr("Save snapshot file"),
	 (""),
	 tr("PNG (*.png);;JPEG (*.JPEG);;Windows Bitmap (*.bmp)")
	 );
	
	printf("export: %s\n", fileName.toStdString().c_str());
	signalSnapshot(fileName, checkBoxTree->checkState() == Qt::Checked, checkBoxAlignment->checkState() == Qt::Checked);
}

void SnapshotWindow::update()
{
	buttonSave->setEnabled(checkBoxTree->checkState() == Qt::Checked || checkBoxAlignment->checkState() == Qt::Checked);
}