//
//  PhylogenyTreeStatusBar.h
//  gavqt
//
//  Created by Brian Ondov on 4/4/13.
//
//

#ifndef __gavqt__PhylogenyTreeStatusBar__
#define __gavqt__PhylogenyTreeStatusBar__

#include <QWidget>
#include "StatusLabel.h"
#include "harvest/PhylogenyTreeNode.h"

class PhylogenyTreeStatusBar : public QWidget
{
public:
	
	PhylogenyTreeStatusBar();
	
	void setName(const QString & name);
	void setPhylogenyTreeNode(const PhylogenyTreeNode * PhylogenyTreeNode, const QString & name, int leaf = -1);
	
private:
	
	const PhylogenyTreeNode * node;
	StatusLabel * labelName;
	StatusLabel * labelDistance;
	StatusLabel * labelBootstrap;
};

#endif /* defined(__gavqt__PhylogenyTreeStatusBar__) */
