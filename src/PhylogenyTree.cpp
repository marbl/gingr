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

PhylogenyTree::~PhylogenyTree()
{
	delete this->root;
}

void PhylogenyTree::getLeafIds(QVector<int> & ids) const
{
	ids.resize(0);
	root->getLeafIds(ids);
}

float PhylogenyTree::leafDistance(int leaf1, int leaf2) const
{
	const PhylogenyNode * node1 = leaves[leaf1];
	const PhylogenyNode * node2 = leaves[leaf2];
	
	float distance = 0;
	
	while ( node1 != node2 )
	{
		if ( node1->getDepth() > node2->getDepth() )
		{
			distance += node1->getDistance();
			node1 = node1->getParent();
		}
		else if ( node2->getDepth() > node1->getDepth() )
		{
			distance += node2->getDistance();
			node2 = node2->getParent();
		}
		else
		{
			distance += node1->getDistance();
			distance += node2->getDistance();
			node1 = node1->getParent();
			node2 = node2->getParent();
		}
	}
	
	return distance;
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

bool PhylogenyTree::loadPb(const Harvest::Tree & msg)
{
	if ( root )
	{
		delete root;
	}
	
	nodeCount = 0;
	int leaf = 0;
	root = new PhylogenyNode(msg.root(), nodeCount, leaf);
	leaves.resize(0);
	root->getLeaves(leaves);
	root->setAlignDist(root->getDistanceMax(), 0);
	return true; // TODO: errors?
}

