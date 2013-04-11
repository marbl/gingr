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
#include "PhylogenyNode.h"

class PhylogenyTreeStatusBar : public QWidget
{
public:
	
	PhylogenyTreeStatusBar();
	
	void setPhylogenyNode(const PhylogenyNode * phylogenyNode, const QString & name, int leaf = -1);
	
private:
	
	const PhylogenyNode * node;
	StatusLabel * labelName;
	StatusLabel * labelDistance;
	StatusLabel * labelBootstrap;
};

#endif /* defined(__gavqt__PhylogenyTreeStatusBar__) */
