//
//  Track.h
//  gavqt
//
//  Created by Brian Ondov on 8/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_Track_h
#define gavqt_Track_h

#include <QString>
#include <QVector>
#include "Region.h"

class Track
{
public:
	
	Track(const QString & name);
	
	void addRegion(const Region * region);
	unsigned int getEnd() const;
	const QString & getName() const;
	const Region * getRegion(int index) const;
	int regionsCount() const;
	void sortRegions();
	
private:
	
	QString name;
	QVector<const Region *> regions;
};


inline const QString & Track::getName() const {return name;}
inline const Region * Track::getRegion(int index) const {return regions[index];}
inline int Track::regionsCount() const {return regions.size();}

#endif
