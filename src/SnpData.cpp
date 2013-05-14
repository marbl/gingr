//
//  SnpData.cpp
//  gavqt
//
//  Created by Brian Ondov on 5/7/13.
//
//

#include "SnpData.h"

SnpData::SnpData(int newBins, int newTrackCount)
:	bins(newBins),
	trackCount(newTrackCount)
{
	snps = new int * [trackCount];
	
	for (int i = 0; i < trackCount; i++)
	{
		snps[i] = new int[bins];
	}
	
	lcbs = new int[bins];
	snpSums = new int[bins];
}

SnpData::~SnpData()
{
	for ( int i = 0; i < trackCount; i++ )
	{
		delete [] snps[i];
	}
	
	delete [] snps;
	delete [] lcbs;
	delete [] snpSums;
}

void SnpData::setWindow(int newPosStart, int newPosEnd)
{
	posStart = newPosStart;
	posEnd = newPosEnd;
}
