//
//  PhylogenyNode.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "PhylogenyNode.h"

#include <stdlib.h>

PhylogenyNode::PhylogenyNode(const Harvest::Tree::Node & msgNode, PhylogenyNode * parent)
{
	// load from protobuf
	
	this->parent = parent;
	childrenCount = msgNode.children_size();
	
	trackId = msgNode.track();
	distance = msgNode.branchlength();
	bootstrap = msgNode.bootstrap();
	
	children = new PhylogenyNode* [childrenCount];
	
	for ( int i = 0; i < childrenCount; i++ )
	{
		children[i] = new PhylogenyNode(msgNode.children(i), this);
	}
	
//	collapse = false;
}

PhylogenyNode::PhylogenyNode(PhylogenyNode * child1, PhylogenyNode * child2)
{
	// edge bisection
	
	distance = 0;
	parent = 0;
	childrenCount = 2;
	bootstrap = 1;
	
	children = new PhylogenyNode * [childrenCount];
	
	children[0] = child1;
	children[1] = child2;
}

PhylogenyNode::~PhylogenyNode()
{
	for ( int i = 0; i < childrenCount; i++ )
	{
		delete children[i];
	}
	
	delete [] children;
}

PhylogenyNode * PhylogenyNode::bisectEdge(float distanceLower)
{
	PhylogenyNode * parentNew = new PhylogenyNode(this, parent);
	
	parent->invert(this);
	parent->setParent(parentNew, distance - distanceLower);
	distance = distanceLower;
	parent = parentNew;
	
	return parentNew;
}

PhylogenyNode * PhylogenyNode::collapse()
{
	childrenCount = 0;
	children[0]->setParent(parent, distance + children[0]->getDistance());
	
	return children[0];
}

void PhylogenyNode::getLeafIds(QVector<int> & ids) const
{
	if ( childrenCount == 0 )
	{
		ids.push_back(trackId);
	}
	else
	{
		for ( int i = 0; i < childrenCount; i++ )
		{
			children[i]->getLeafIds(ids);
		}
	}
}

void PhylogenyNode::getLeaves(QVector<const PhylogenyNode *> & leaves) const
{
	if ( childrenCount == 0 )
	{
		leaves.push_back(this);
	}
	else
	{
		for ( int i = 0; i < childrenCount; i++ )
		{
			children[i]->getLeaves(leaves);
		}
	}
}

void PhylogenyNode::getPairwiseDistances(float ** matrix, int size)
{
	for ( int i = 0; i < size; i++ )
	{
		if ( i < leafMin || i > leafMax )
		{
			for ( int j = leafMin; j <= leafMax; j++ )
			{
				int row;
				int col;
				
				if ( i > j )
				{
					row = i;
					col = j;
				}
				else
				{
					row = j;
					col = i;
				}
				
				matrix[row - 1][col] += distance;
			}
		}
	}
	
	for ( int i = 0; i < childrenCount; i++ )
	{
		children[i]->getPairwiseDistances(matrix, size);
	}
}

void PhylogenyNode::initialize(int & newId, int &leaf, float depthParent)
{
	id = newId;
	newId++;
	leafMin = leaf;
	distanceMax = distance;
	depth = depthParent + distance;
	double distanceMaxChild = 0;
	
	for ( int i = 0; i < childrenCount; i++ )
	{
		children[i]->initialize(newId, leaf, depth);
		
		if( children[i]->getDistanceMax() > distanceMaxChild )
		{
			distanceMaxChild = children[i]->getDistanceMax();
		}
	}
	
	if ( childrenCount == 0 )
	{
		leaf++;
	}
	
	leafMax = leaf - 1;
	distanceMax += distanceMaxChild;
}

void PhylogenyNode::invert(PhylogenyNode * fromChild)
{
	QVector<PhylogenyNode *> childrenNew;
	
	for ( int i = 0; i < childrenCount; i++ )
	{
		if ( children[i] != fromChild )
		{
			childrenNew.push_back(children[i]);
		}
	}
	
	if ( parent )
	{
		childrenNew.push_back(parent);
	}
	
	delete [] children;
	childrenCount = childrenNew.count();
	children = new PhylogenyNode * [childrenCount];
	
	for ( int i = 0; i < childrenCount; i++ )
	{
		children[i] = childrenNew.at(i);
	}
	
	if ( parent )
	{
		parent->invert(this);
		printf("children: %d\n", parent->getChildrenCount());
		if ( parent->getChildrenCount() == 1 )
		{
			children[childrenCount - 1] = parent->collapse();
			delete parent;
		}
	}
	
	if ( fromChild )
	{
		distance = fromChild->getDistance();
	}
	
	parent = fromChild;
}

void PhylogenyNode::setAlignDist(float dist, float dep)
{
	distanceAlign = dist - (distanceMax);
	depthAlign = dep + distance + distanceAlign;
	
	for ( int i = 0; i < childrenCount; i++ )
	{
		children[i]->setAlignDist(distanceMax - distance, depthAlign);
	}
}

void PhylogenyNode::setParent(PhylogenyNode *parentNew, float distanceNew)
{
	parent = parentNew;
	distance = distanceNew;
}

void PhylogenyNode::swapSiblings()
{
	PhylogenyNode * temp = children[0];
	children[0] = children[1];
	children[1] = temp;
}
