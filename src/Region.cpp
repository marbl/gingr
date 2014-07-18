// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

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
