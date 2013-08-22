//
//  SnpData.h
//  gavqt
//
//  Created by Brian Ondov on 5/7/13.
//
//

#ifndef __gavqt__SnpData__
#define __gavqt__SnpData__

#include <QImage>

class SnpData
{
	
public:
	
	SnpData(int newBins, int newTrackCount);
	~SnpData();
	
	int getBins() const;
	int * getLcbs() const;
	int getPosEnd() const;
	int getPosStart() const;
	QImage * getRow(int row) const;
	QImage * getRowSmall(int row) const;
	int getSnpMax() const;
	int * getSnps(int row) const;
	QImage * getSum() const;
	void setSnpMax(int max);
	void setWindow(int newPosStart, int newPosEnd);
	
private:
	
	int ** snps;
	QImage ** rows;
	QImage ** rowsSmall;
	QImage * sum;
	int * lcbs;
	int posStart;
	int posEnd;
	int bins;
	int snpMax;
	int trackCount;
};

inline int SnpData::getBins() const {return bins;}
inline int * SnpData::getLcbs() const {return lcbs;}
inline int SnpData::getPosEnd() const {return posEnd;}
inline int SnpData::getPosStart() const {return posStart;}
inline QImage * SnpData::getRow(int row) const {return rows[row];}
inline QImage * SnpData::getRowSmall(int row) const {return rowsSmall[row];}
inline int SnpData::getSnpMax() const {return snpMax;}
inline int * SnpData::getSnps(int row) const {return snps[row];}
inline QImage * SnpData::getSum() const {return sum;}
inline void SnpData::setSnpMax(int max) {snpMax = max;}

#endif /* defined(__gavqt__SnpData__) */
