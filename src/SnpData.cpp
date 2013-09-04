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
	rows = new QImage * [trackCount];
	rowsSmall = new QImage * [trackCount];
	
	for (int i = 0; i < trackCount; i++)
	{
		snps[i] = new int [bins];
		rows[i] = new QImage(bins, 1, QImage::Format_RGB32);
		rowsSmall[i] = new QImage(bins / 2, 1, QImage::Format_RGB32);
	}
	
	lcbs = new int[bins];
	sum = new QImage(bins, 1, QImage::Format_RGB32);
}

SnpData::~SnpData()
{
	for ( int i = 0; i < trackCount; i++ )
	{
		delete [] snps[i];
		delete rowsSmall[i];
	}
	
	delete [] snps;
	delete [] rowsSmall;
	delete [] lcbs;
	delete sum;
}

void SnpData::setFilters(unsigned int newFilters, unsigned int newFiltersScale, bool newFilterPass, bool newFilterPassScale)
{
	filters = newFilters;
	filtersScale = newFiltersScale;
	filterPass = newFilterPass;
	filterPassScale = newFilterPassScale;
}

void SnpData::setWindow(int newPosStart, int newPosEnd)
{
	posStart = newPosStart;
	posEnd = newPosEnd;
}
