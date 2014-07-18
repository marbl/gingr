// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__SnpData__
#define __gavqt__SnpData__

#include <QImage>

class SnpData
{
	
public:
	
	SnpData(int newBins, int newTrackCount);
	~SnpData();
	
	int getBins() const;
	unsigned int getFilters() const;
	unsigned int getFiltersScale() const;
	bool getFilterPass() const;
	bool getFilterPassScale() const;
	int getGapMax() const;
	int * getGaps(int row) const;
	int * getLcbs() const;
	bool getLightColors() const;
	int getPosEnd() const;
	int getPosStart() const;
	QImage * getRow(int row) const;
	QImage * getRowSmall(int row) const;
	int getShowGaps() const;
	int getSnpMax() const;
	int getSnpMaxView() const;
	int * getSnps(int row) const;
	int * getSnpsScale(int row) const;
	QImage * getSum() const;
	bool getSynteny() const;
	int getTrackMin() const;
	int getTrackMax() const;
	void setFilters(unsigned int newFilters, unsigned int newFiltersScale, bool newFilterPass, bool newFilterPassScale);
	void setGapMax(int max);
	void setLightColors(bool light);
	void setTracks(int trackMinNew, int trackMaxNew);
	void setShowGaps(int gaps);
	void setSnpMax(int max);
	void setSnpMaxView(int max);
	void setSynteny(bool newSynteny);
	void setWindow(int newPosStart, int newPosEnd);
	
private:
	
	int ** snps;
	int ** gaps;
	int ** snpsScale;
	QImage ** rows;
	QImage ** rowsSmall;
	QImage * sum;
	int * lcbs;
	int posStart;
	int posEnd;
	int bins;
	int trackMin;
	int trackMax;
	int snpMax;
	int snpMaxView;
	int gapMax;
	int trackCount;
	bool synteny;
	bool lightColors;
	int showGaps;
	unsigned int filters;
	unsigned int filtersScale;
	bool filterPass;
	bool filterPassScale;
};

inline int SnpData::getBins() const {return bins;}
inline unsigned int SnpData::getFilters() const {return filters;}
inline unsigned int SnpData::getFiltersScale() const {return filtersScale;}
inline bool SnpData::getFilterPass() const {return filterPass;}
inline bool SnpData::getFilterPassScale() const {return filterPassScale;}
inline int SnpData::getGapMax() const {return gapMax;}
inline int * SnpData::getGaps(int row) const {return gaps[row];}
inline int * SnpData::getLcbs() const {return lcbs;}
inline bool SnpData::getLightColors() const {return lightColors;}
inline int SnpData::getPosEnd() const {return posEnd;}
inline int SnpData::getPosStart() const {return posStart;}
inline QImage * SnpData::getRow(int row) const {return rows[row];}
inline QImage * SnpData::getRowSmall(int row) const {return rowsSmall[row];}
inline int SnpData::getShowGaps() const {return showGaps;}
inline int SnpData::getSnpMax() const {return snpMax;}
inline int SnpData::getSnpMaxView() const {return snpMaxView;}
inline int * SnpData::getSnps(int row) const {return snps[row];}
inline int * SnpData::getSnpsScale(int row) const {return snpsScale[row];}
inline QImage * SnpData::getSum() const {return sum;}
inline bool SnpData::getSynteny() const {return synteny;}
inline int SnpData::getTrackMin() const {return trackMin;}
inline int SnpData::getTrackMax() const {return trackMax;}
inline void SnpData::setGapMax(int max) {gapMax = max;}
inline void SnpData::setLightColors(bool light) {lightColors = light;}
inline void SnpData::setShowGaps(int gaps) {showGaps = gaps;}
inline void SnpData::setSnpMax(int max) {snpMax = max;}
inline void SnpData::setSnpMaxView(int max) {snpMaxView = max;}
inline void SnpData::setSynteny(bool newSynteny) {synteny = newSynteny;}

#endif /* defined(__gavqt__SnpData__) */
