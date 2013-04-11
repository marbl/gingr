//
//  Region.h
//  gavqt
//
//  Created by Brian Ondov on 8/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_Region_h
#define gavqt_Region_h

class Region
{
public:
	
	Region(int newLcb, unsigned int newStart, unsigned int newLength, bool newRc, char * newSnps);
	~Region();
	
	float getEndScaled() const;
	int getLcb() const;
	unsigned int getStart() const;
	float getStartScaled() const;
	unsigned int getLength() const;
	bool getRc() const;
	char getSnp(unsigned int snp) const;
	static bool lessThan(const Region* r1, const Region* r2);
	void setScaled(float start, float end);
 //	bool operator<(const Region& other) const;
	
private:
	
	int lcb;
	unsigned int start;
	unsigned int length;
	float startScaled;
	float endScaled;
	char * snps;
	bool rc;
};

inline float Region::getEndScaled() const {return endScaled;}
inline int Region::getLcb() const {return lcb;}
inline unsigned int Region::getStart() const {return start;}
inline float Region::getStartScaled() const {return startScaled;}
inline unsigned int Region::getLength() const {return length;}
inline bool Region::getRc() const {return rc;}
inline char Region::getSnp(unsigned int snp) const {return snps[snp];}
//inline bool Region::operator<(const Region &other) const {return start < other.getStart();}

#endif
