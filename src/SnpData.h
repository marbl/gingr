//
//  SnpData.h
//  gavqt
//
//  Created by Brian Ondov on 5/7/13.
//
//

#ifndef __gavqt__SnpData__
#define __gavqt__SnpData__

class SnpData
{
	
public:
	
	SnpData(int newBins, int newTrackCount);
	~SnpData();
	
	int getBins() const;
	int * getLcbs() const;
	int getPosEnd() const;
	int getPosStart() const;
	int getSnpMax() const;
	int getSnpSumMax() const;
	int ** getSnps() const;
	int * getSnpSums() const;
	void setSnpMax(int max);
	void setSnpSumMax(int max);
	void setWindow(int newPosStart, int newPosEnd);
	
private:
	
	int ** snps;
	int * snpSums;
	int * lcbs;
	int posStart;
	int posEnd;
	int bins;
	int snpMax;
	int snpSumMax;
	int trackCount;
};

inline int SnpData::getBins() const {return bins;}
inline int * SnpData::getLcbs() const {return lcbs;}
inline int SnpData::getPosEnd() const {return posEnd;}
inline int SnpData::getPosStart() const {return posStart;}
inline int ** SnpData::getSnps() const {return snps;}
inline int * SnpData::getSnpSums() const {return snpSums;}
inline int SnpData::getSnpMax() const {return snpMax;}
inline int SnpData::getSnpSumMax() const {return snpSumMax;}
inline void SnpData::setSnpMax(int max) {snpMax = max;}
inline void SnpData::setSnpSumMax(int max) {snpSumMax = max;}

#endif /* defined(__gavqt__SnpData__) */
