//
//  PhylogenyNode.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "PhylogenyNode.h"

#include <stdlib.h>

PhylogenyNode::PhylogenyNode(const QDomElement* element, int & newId, int & leaf, const PhylogenyNode * parent, float depth)
{
	// first, count children
	
//	leafCount = 0;
	this->parent = parent;
	childrenCount = 0;
	id = newId;
	newId++;
	
	for(QDomElement child = element->firstChildElement("node"); !child.isNull(); child = child.nextSiblingElement("node"))
	{
		childrenCount++;
	}
	
	leafMin = leaf;
	
	trackId = element->attribute("id").toInt();
	distance = element->attribute("dist").toFloat();
	bootstrap = element->attribute("boot").toFloat();
	
	distanceMax = distance;
	this->depth = depth + distance;
	children = new PhylogenyNode* [childrenCount];
	unsigned int i = 0;
	double distanceMaxChild = 0;
	
	for( QDomElement child = element->firstChildElement("node"); !child.isNull(); child = child.nextSiblingElement("node") )
	{
		children[i] = new PhylogenyNode(&child, newId, leaf, this, this->depth);
//		leafCount += children[i]->getLeafCount();
		
		if( children[i]->getDistanceMax() > distanceMaxChild )
		{
			distanceMaxChild = children[i]->getDistanceMax();
		}
		
		i++;
	}
	
	if ( childrenCount == 0 )
	{
//		leafCount = 1;
		leaf++;
	}
	
	leafMax = leaf - 1;
	
	distanceMax += distanceMaxChild;
	collapse = false;
}

PhylogenyNode::PhylogenyNode(const Harvest::Tree::Node & msgNode, int & newId, int & leaf, const PhylogenyNode * parent, float depth)
{
	// first, count children
	
	//	leafCount = 0;
	this->parent = parent;
	childrenCount = msgNode.children_size();
	id = newId;
	newId++;
	
	leafMin = leaf;
	
	trackId = msgNode.track();
	distance = msgNode.branchlength();
	bootstrap = msgNode.bootstrap();
	
	distanceMax = distance;
	this->depth = depth + distance;
	children = new PhylogenyNode* [childrenCount];
	double distanceMaxChild = 0;
	
	for ( int i = 0; i < childrenCount; i++ )
	{
		children[i] = new PhylogenyNode(msgNode.children(i), newId, leaf, this, this->depth);
		
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
	collapse = false;
}

PhylogenyNode::~PhylogenyNode()
{
	for ( int i = 0; i < childrenCount; i++ )
	{
		delete children[i];
	}
	
	delete [] children;
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

void PhylogenyNode::setAlignDist(float dist, float dep)
{
	distanceAlign = dist - (distanceMax);
	depthAlign = dep + distance + distanceAlign;
	
	for ( int i = 0; i < childrenCount; i++ )
	{
		children[i]->setAlignDist(distanceMax - distance, depthAlign);
	}
}
