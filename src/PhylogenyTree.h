//
//  PhylogenyTree.h
//  gavqt
//
//  Created by Brian Ondov on 8/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_PhylogenyTree_h
#define gavqt_PhylogenyTree_h

#include "PhylogenyNode.h"
#include <QDomDocument>

class PhylogenyTree
{
public:
	
	PhylogenyTree();
	~PhylogenyTree();
	
	const PhylogenyNode * getLeaf(int id) const;
	void getLeafIds(QVector<int> & ids) const;
	int getNodeCount() const;
	float leafDistance(int leaf1, int leaf2) const;
	bool loadDom(const QDomElement* documentElement);
	bool loadPb(const Harvest::Tree & msg);
	
	PhylogenyNode * getRoot() const;
	
private:
	
	QVector<const PhylogenyNode *> leaves;
	PhylogenyNode * root;
	int nodeCount;
};

inline const PhylogenyNode * PhylogenyTree::getLeaf(int id) const {return leaves[id];}
inline int PhylogenyTree::getNodeCount() const {return nodeCount;}
inline PhylogenyNode * PhylogenyTree::getRoot() const {return this->root;}


#endif
