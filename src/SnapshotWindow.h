//
//  SnapshotWindow.h
//  gingr
//
//  Created by Brian Ondov on 7/23/13.
//
//

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
