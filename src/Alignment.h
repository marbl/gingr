//
//  Alignment.h
//  gavqt
//
//  Created by Brian Ondov on 8/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_Alignment_h
#define gavqt_Alignment_h

#include <QVector>
#include <QDomDocument>
#include "Region.h"
#include <QString>

typedef QVector<Region *> RegionVector;
typedef QVector<RegionVector *> RegionTable;

class Alignment
{
public:
	
	struct Snp
	{
		int pos;
		unsigned int filters;
		//char ref;
		char snp;
	};
	
	struct Filter
	{
		unsigned int flag;
		QString name;
		QString description;
	};
	
	struct Gap
	{
		int start;
		int end;
		int startAbs;
		int offset;
	};
	
	struct Lcb
	{
		Lcb() {regions = 0; lengthGapped = 0; concordance = 0;}
		
		RegionVector * regions;
		int lengthGapped;
		float concordance;
		int number;
	};
	
	struct Position
	{
		int abs; // absolute reference position
		int gap; // position along insertion after abs
	};
	
	Alignment();
	~Alignment();
	void disableFilter(int index);
	void enableFilter(int index);
	bool filter(int flags) const;
	bool filterScale(int flags) const;
	const Filter & getFilter(int index) const;
	int getFilterCount() const;
	unsigned int getFilterFlags() const;
//	unsigned int getNextSnpIndex(const Window & window) const;
	unsigned int getNextSnpIndex(unsigned int track, unsigned int pos) const;
	unsigned int getNextSnpIndex(unsigned int pos) const;
	const RegionTable * getTracks() const;
	const Lcb & getLcb(int index) const;
	int getLcbCount() const;
	unsigned int getLength() const;
	unsigned int getPositionGapped(unsigned int ungapped) const;
	Position getPositionUngapped(unsigned int gapped) const;
	const char * getRefSeqGapped() const;
	const unsigned int * getSnpPositionsByLcb(int lcb) const;
	const Snp & getSnp(int track, unsigned int snp) const;
	unsigned int getSnpCount() const;
	unsigned int getSnpCountByLcb(int lcb) const;
	unsigned int getSnpCountByTrack(int track) const;
	unsigned int getSnpPosition(int index) const;
	int getSnpCountByPosition(int index) const;
	const Snp & getSnpByPosition(int indexPos, int indexTrack) const;
	bool loadDom(const QDomElement * elementAlignment);
	void setFilterPass(bool pass);
	void setFilterScale();
	void setFilterShow(bool show);
	
private:
	
	void computeTrackSnps();
	
	// TODO: Get rid of RegionTable and use Track and Lcb objects
	//
	RegionTable tracks;
	QVector<Lcb> lcbs;
//	QVector<unsigned int *> snpPositions;
	QVector<unsigned int> snpCounts;
	QVector<Gap> gaps;
	unsigned int gapsTotal;
	QVector<Snp> ** snpsByTrack;
	QVector<unsigned int> snpPositions;
	unsigned int snpCount;
	QVector<QVector<Snp> > snpsByPos;
	QVector<Filter> filters;
	unsigned int filterFlags;
	unsigned int filterFlagsScale;
	bool filterShow;
	bool filterPass;
	bool filterPassScale;
	char * refSeqGapped;
	unsigned int * snpCountsByTrack;
	unsigned int totalLength;
};

inline void Alignment::disableFilter(int index) {filterFlags = filterFlags & ~filters[index].flag;}
inline void Alignment::enableFilter(int index) {filterFlags = filterFlags | filters[index].flag;}
inline const Alignment::Filter & Alignment::getFilter(int index) const {return filters[index];}
inline int Alignment::getFilterCount() const {return filters.size();}
inline unsigned int Alignment::getFilterFlags() const {return filterFlags;}
inline const RegionTable * Alignment::getTracks() const {return &tracks;}
inline const Alignment::Lcb & Alignment::getLcb(int index) const {return lcbs[index];}
inline int Alignment::getLcbCount() const {return lcbs.size();}
inline unsigned int Alignment::getLength() const {return totalLength;}
inline const char * Alignment::getRefSeqGapped() const {return refSeqGapped;}
inline const Alignment::Snp & Alignment::getSnp(int track, unsigned int snp) const {return (*snpsByTrack[track])[snp];}
//inline const unsigned int * Alignment::getSnpPositionsByLcb(int lcb) const {return snpPositions[lcb];};
inline unsigned int Alignment::getSnpPosition(int index) const {return snpPositions[index];};
inline unsigned int Alignment::getSnpCount() const {return snpPositions.size();}
inline unsigned int Alignment::getSnpCountByLcb(int lcb) const {return snpCounts[lcb];}
//inline unsigned int Alignment::getSnpCountByTrack(int track) const {return snpCountsByTrack[track];}
inline unsigned int Alignment::getSnpCountByTrack(int track) const {return (*snpsByTrack[track]).size();}
inline int Alignment::getSnpCountByPosition(int index) const {return snpsByPos[index].size();}
inline const Alignment::Snp & Alignment::getSnpByPosition(int indexPos, int indexTrack) const {return snpsByPos[indexPos][indexTrack];}
inline void Alignment::setFilterPass(bool pass) {filterPass = pass;}
inline void Alignment::setFilterScale() {filterFlagsScale = filterFlags; filterPassScale = filterPass;}
inline void Alignment::setFilterShow(bool show) {filterShow = show;}

#endif
