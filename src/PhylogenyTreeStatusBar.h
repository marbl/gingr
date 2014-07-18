// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

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
