// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gingr__SnapshotWindow__
#define __gingr__SnapshotWindow__

#include <QDialog>
#include <QCheckBox>

class SnapshotWindow : public QDialog
{
	Q_OBJECT
	
public:
	
	SnapshotWindow(QWidget * parent);
	
public slots:
	
	void cancel();
	void save();
	void update();
	
signals:
	
	void signalSnapshot(const QString & fileName, bool tree, bool alignment);
	
private:
	
	QCheckBox * checkBoxTree;
	QCheckBox * checkBoxAlignment;
	
	QPushButton * buttonSave;
};

#endif /* defined(__gingr__SnapshotWindow__) */
