//
//  PhylogenyNode.h
//  gavqt
//
//  Created by Brian Ondov on 8/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_PhylogenyNode_h
#define gavqt_PhylogenyNode_h

#include <QDomDocument>
#include <QVector>
#include "HarvestIO.h"

class PhylogenyNode
{
public:
	
	PhylogenyNode(const QDomElement* element, int & newId, int & leaf, const PhylogenyNode * parent = 0, float depth = 0);
	PhylogenyNode(const Harvest::Tree::Node & msgNode, int & newId, int & leaf, const PhylogenyNode * parent = 0, float depth = 0);
	~PhylogenyNode();
	
	float getBootstrap() const;
	PhylogenyNode * getChild(unsigned int index) const;
	int getChildrenCount() const;
	bool getCollapse() const;
	float getDepth() const;
	float getDepthAlign() const;
	double getDistance() const;
	float getDistanceAlign() const;
	int getId() const;
	int getTrackId() const;
	double getDistanceMax() const;
	int getLeafCount() const;
	int getLeafMax() const;
	int getLeafMin() const;
	void getLeaves(QVector<const PhylogenyNode *> & leaves) const;
	void getLeafIds(QVector<int> & ids) const;
	const PhylogenyNode * getParent() const;
	void setAlignDist(float dist, float dep);
	void setCollapse(bool newCollapse);
	
private:
	
	PhylogenyNode** children;
	const PhylogenyNode * parent;
	int childrenCount;
	int id;
	int trackId;
	double distance;
	double distanceMax;
	float depth;
	float depthAlign;
	float distanceAlign;
//	int leafCount;
	int leafMin;
	int leafMax;
	float bootstrap;
	bool collapse;
};

inline float PhylogenyNode::getBootstrap() const {return bootstrap;}
inline PhylogenyNode * PhylogenyNode::getChild(unsigned int index) const {return children[index];};
inline int PhylogenyNode::getChildrenCount() const {return childrenCount;}
inline bool PhylogenyNode::getCollapse() const {return collapse;}
inline float PhylogenyNode::getDepth() const {return depth;}
inline float PhylogenyNode::getDepthAlign() const {return depthAlign;}
inline double PhylogenyNode::getDistance() const {return distance;}
inline int PhylogenyNode::getId() const {return id;}
inline int PhylogenyNode::getTrackId() const {return trackId;}
inline float PhylogenyNode::getDistanceAlign() const {return distanceAlign;}
inline double PhylogenyNode::getDistanceMax() const {return distanceMax;}
inline int PhylogenyNode::getLeafCount() const {return leafMax - leafMin + 1;}
inline int PhylogenyNode::getLeafMax() const {return leafMax;}
inline int PhylogenyNode::getLeafMin() const {return leafMin;}
inline const PhylogenyNode * PhylogenyNode::getParent() const {return parent;}
inline void PhylogenyNode::setCollapse(bool newCollapse) {collapse = newCollapse;}

#endif
