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

bool PhylogenyTree::loadPb(const Harvest::Tree & msg)
{
	if ( root )
	{
		delete root;
	}
	
	nodeCount = 0;
	int leaf = 0;
	root = new PhylogenyNode(msg.root());
	leaves.resize(0);
	root->initialize(nodeCount, leaf);
	root->getLeaves(leaves);
	root->setAlignDist(root->getDistanceMax(), 0);
	return true; // TODO: errors?
}

void PhylogenyTree::midpointReroot()
{
	// lower triangular matrix of pairwise distances between leaves
	//
	int leavesCount = leaves.size();
	float ** distance = new float*[leavesCount - 1];
	
	for ( int i = 0; i < leavesCount - 1; i++ )
	{
		distance[i] = new float[i + 1];
		memset(distance[i], 0, sizeof(float) * (i + 1));
	}
	
	root->getPairwiseDistances(distance, leavesCount);
	
	float max = 0;
	int maxLeaf1;
	int maxLeaf2;
	
	for ( int i = 0; i < leavesCount - 1; i++ )
	{
		for ( int j = 0; j < i + 1; j++ )
		{
			if ( distance[i][j] > max )
			{
				max = distance[i][j];
				maxLeaf1 = i + 1;
				maxLeaf2 = j;
			}
		}
	}
	
	float midDistance = distance[maxLeaf1 - 1][maxLeaf2] / 2;
	printf("%f\t%f\t%f\t%d\t%d\n", max, distance[maxLeaf1 - 1][maxLeaf2], midDistance, maxLeaf1, maxLeaf2);
	
	for ( int i = 0; i < leavesCount - 1; i++ )
	{
		delete [] distance[i];
	}
	
	delete [] distance;
	
	const PhylogenyNode * node;
	
	if ( leaves[maxLeaf1]->getDepth() > leaves[maxLeaf2]->getDepth() )
	{
		node = leaves[maxLeaf1];
	}
	else
	{
		node = leaves[maxLeaf2];
	}
	
	float depth = 0;
	
	while ( depth + node->getDistance() < midDistance )
	{
		depth += node->getDistance();
		node = node->getParent();
	}
	
	reroot(node, midDistance - depth);
}

void PhylogenyTree::setOutgroup(const PhylogenyNode * node)
{
	reroot(node, node->getParent() == root ? node->getDistance() : node->getDistance() / 2);
}

void PhylogenyTree::reroot(const PhylogenyNode * rootNew, float distance)
{
	int leaf = 0;
	nodeCount = 0;
	
	if ( rootNew->getParent() == root )
	{
		PhylogenyNode * rootNewMutable;
		PhylogenyNode * sibling;
		
		if ( root->getChild(0) == rootNew )
		{
			rootNewMutable = root->getChild(0);
			sibling = root->getChild(1);
		}
		else
		{
			sibling = root->getChild(0);
			rootNewMutable = root->getChild(1);
			
			root->swapSiblings();
		}
		
		sibling->setParent(root, rootNew->getDistance() + sibling->getDistance() - distance);
		rootNewMutable->setParent(root, distance);
	}
	else
	{
		root = const_cast<PhylogenyNode *>(rootNew)->bisectEdge(distance);
	}
	
	root->initialize(nodeCount, leaf);
	leaves.resize(0);
	root->getLeaves(leaves);
	root->setAlignDist(root->getDistanceMax(), 0);
}
