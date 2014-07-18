// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "SnpData.h"

SnpData::SnpData(int newBins, int newTrackCount)
:	bins(newBins),
	trackCount(newTrackCount)
{
	snps = new int * [trackCount];
	gaps = new int * [trackCount];
	snpsScale = new int * [trackCount];
	rows = new QImage * [trackCount];
	rowsSmall = new QImage * [trackCount];
	
	for (int i = 0; i < trackCount; i++)
	{
		snps[i] = new int [bins];
		gaps[i] = new int [bins];
		snpsScale[i] = new int [bins];
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
		delete [] gaps[i];
		delete [] snpsScale[i];
		delete rows[i];
		delete rowsSmall[i];
	}
	
	delete [] snps;
	delete [] gaps;
	delete [] snpsScale;
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

void SnpData::setTracks(int trackMinNew, int trackMaxNew)
{
	trackMin = trackMinNew;
	trackMax = trackMaxNew;
}

void SnpData::setWindow(int newPosStart, int newPosEnd)
{
	posStart = newPosStart;
	posEnd = newPosEnd;
}
