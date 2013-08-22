//
//  Region.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Region.h"

using namespace gav;

Region::Region(int newLcb, unsigned int newStart, unsigned int newLength, bool newRc, char * newSnps)
{
	lcb = newLcb;
	start = newStart;
	length = newLength;
	rc = newRc;
	snps = newSnps;
}

Region::~Region()
{
	if ( snps )
	{
		delete [] snps;
	}
}

bool Region::lessThan(const Region* r1, const Region* r2)
{
	return r1->getStart() < r2->getStart();
}

void Region::setScaled(float start, float end)
{
	startScaled = start;
	endScaled = end;
}
