// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef gavqt_Alignment_h
#define gavqt_Alignment_h

#include <QVector>
#include <QDomDocument>
#include "Region.h"
#include <QString>
#include "harvest/HarvestIO.h"
#include <QMap>
#include <stdexcept>

typedef QVector<gav::Region *> RegionVector;
typedef QVector<RegionVector *> RegionTable;

class Alignment
{
public:
	
	enum GapFlag
	{
		SHOW = 1,
		INSERTIONS = 2,
		DELETIONS = 4,
	};
	
	struct Snp
	{
		int track;
		char snp;
	};
	
	struct SnpColumn
	{
		int position;
		char ref;
		unsigned int filters;
		QVector<Snp> snps;
	};
	
	typedef std::map<long long int, int> SnpMap;
	
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
		long long int startGapped;
		float concordance;
		int number;
	};
	
	struct Position
	{
		int abs; // absolute reference position
		int gap; // position along insertion after abs, from 1
	};
	
	Alignment();
	~Alignment();
	
	void clear();
	void disableFilter(int index);
	void enableFilter(int index);
	bool filter(unsigned int flags) const;
	bool filter(unsigned int flags, unsigned int filters, bool pass) const;
	bool filterScale(int flags) const;
	const Filter & getFilter(int index) const;
	float getCore() const;
	int getFilterCount() const;
	bool getFilterPass() const;
	bool getFilterPassScale() const;
	unsigned int getFilters() const;
	unsigned int getFiltersScale() const;
	int getNextLcb(int gapped) const;
//	int getNextSnpIndex(int track, int pos) const;
//	Alignment::SnpMap::iterator getSnpEnd(int track) const;
	int getNextSnpIndex(int pos) const;
	const RegionTable * getTracks() const;
	const Lcb & getLcb(int index) const;
	int getLcbCount() const;
	int getLength() const;
	long long int getPositionGapped(long long int ungapped) const;
	Position getPositionUngapped(long long int gapped) const;
	int getRefSeqCount() const;
	const char * getRefSeqGapped() const;
	long long int getRefSeqStart(int seq) const;
	const SnpColumn & getSnpColumn(int index) const;
	int getSnpColumnCount() const;
	int getTrackReference() const;
	bool init(const LcbList & lcbList, const VariantList & variantList, const ReferenceList & referenceList, const TrackList & trackList);
	void setFilterPass(bool pass);
	void setFilterScale();
	void setFilterShow(bool show);
	void setTrackReference(int trackReferenceNew);
	
private:
	
	void destroyRegions();
	
	// TODO: Get rid of RegionTable and use Track and Lcb objects
	//
	RegionTable tracks;
	QVector<Lcb> lcbs;
	QVector<Gap> gaps;
	int gapsTotal;
	SnpMap snpMap;
	QVector<SnpColumn> snpColumns;
	int snpCount;
	QVector<Filter> filters;
	unsigned int filterFlags;
	unsigned int filterFlagsScale;
	bool filterShow;
	bool filterPass;
	bool filterPassScale;
	char * refSeqGapped;
	long long int totalLength;
	int refSeqCount;
	long long int * refSeqStarts;
	float core;
	int trackReference;
};

inline void Alignment::disableFilter(int index) {filterFlags = filterFlags & ~filters[index].flag;}
inline void Alignment::enableFilter(int index) {filterFlags = filterFlags | filters[index].flag;}
inline float Alignment::getCore() const {return core;}
inline const Alignment::Filter & Alignment::getFilter(int index) const {return filters[index];}
inline int Alignment::getFilterCount() const {return filters.size();}
inline bool Alignment::getFilterPass() const {return filterPass;}
inline bool Alignment::getFilterPassScale() const {return filterPassScale;}
inline unsigned int Alignment::getFilters() const {return filterFlags;}
inline unsigned int Alignment::getFiltersScale() const {return filterFlagsScale;}
inline const RegionTable * Alignment::getTracks() const {return &tracks;}
inline const Alignment::Lcb & Alignment::getLcb(int index) const {try{return lcbs[index];		}catch (const std::out_of_range& oor) {
	std::cerr << "Out of Range error: " << oor.what() << '\n';
}
}
inline int Alignment::getLcbCount() const {return lcbs.size();}
inline int Alignment::getLength() const {return totalLength;}
inline int Alignment::getRefSeqCount() const {return refSeqCount;}
inline const char * Alignment::getRefSeqGapped() const {return refSeqGapped;}
inline long long int Alignment::getRefSeqStart(int seq) const {return refSeqStarts[seq];}
inline const Alignment::SnpColumn & Alignment::getSnpColumn(int index) const {try{return snpColumns.at(index);		}catch (const std::out_of_range& oor) {
	std::cerr << "Out of Range error: " << oor.what() << '\n';
}
};
inline int Alignment::getSnpColumnCount() const {return snpColumns.size();}
inline int Alignment::getTrackReference() const {return trackReference;}
inline void Alignment::setFilterPass(bool pass) {filterPass = pass;}
inline void Alignment::setFilterScale() {filterFlagsScale = filterFlags; filterPassScale = filterPass;}
inline void Alignment::setFilterShow(bool show) {filterShow = show;}

inline bool lcbLessThan(const Alignment::Lcb & a, const Alignment::Lcb & b) {return a.startGapped < b.startGapped;}

#endif
