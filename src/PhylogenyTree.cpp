//
//  PhylogenyTree.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "PhylogenyTree.h"
#include <QFile>
#include <QMessageBox>

PhylogenyTree::PhylogenyTree()
{
	root = 0;
}

void PhylogenyTree::getLeafIds(QVector<int> & ids) const
{
	root->getLeafIds(ids);
}

bool PhylogenyTree::loadDom(const QDomElement* documentElement)
{
	if ( root )
	{
		delete root;
	}
	
	nodeCount = 0;
	int leaf = 0;
	root = new PhylogenyNode(documentElement, nodeCount, leaf);
	leaves.resize(0);
	root->getLeaves(leaves);
	return true; // TODO: errors?
}

PhylogenyTree::~PhylogenyTree()
{
	delete this->root;
}
