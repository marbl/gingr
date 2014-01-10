//
//  Alignment.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Alignment.h"
#include <QObject>
#include <QStringList>

using namespace gav;

Alignment::Alignment()
{
	totalLength = 0;
}

Alignment::~Alignment()
{
	// TODO: delete all members of tracks and lcbs
}

bool Alignment::filter(unsigned int flags) const
{
	return filter(flags, filterFlags, filterPass);
}

bool Alignment::filter(unsigned int flags, unsigned int filters, bool pass) const
{
	return (pass && flags == 0) || flags & filters;
}

int Alignment::getNextLcb(int gapped) const
{
	int i = (float)gapped / totalLength * getLcbCount();
	
	if ( i < 0 )
	{
		i = 0;
	}
	
	while ( i > 0 && lcbs[i].startGapped > gapped )
	{
		i--;
	}
	
	while ( i < getLcbCount() && lcbs[i].startGapped + lcbs[i].lengthGapped - 1 < gapped )
	{
		i++;
	}
	
	return i;
}

int Alignment::getNextSnpIndex(int pos) const
{
	return snpMap.lower_bound(pos)->second;
}

long long int Alignment::getPositionGapped(long long int ungapped) const
{
	int i = ungapped * (float)gaps.size() / (totalLength - gapsTotal);
	
	if ( i >= gaps.size() )
	{
		i = gaps.size() - 1;
	}
	
	while ( i < gaps.size() - 1 && gaps[i + 1].startAbs < ungapped )
	{
		i++;
	}
	
	while ( i > 0 && gaps[i].startAbs > ungapped )
	{
		i--;
	}
	
	if ( ungapped >= gaps[i].startAbs )
	{
		return ungapped + gaps[i].offset;
	}
	else // before first gap
	{
		return ungapped;
	}
}

Alignment::Position Alignment::getPositionUngapped(long long int gapped) const
{
	Position pos;
	
	pos.abs = gapped;
	pos.gap = 0;
	
	int i = gapped * (float)gaps.size() / totalLength;
	
	if ( i >= gaps.size() )
	{
		i = gaps.size() - 1;
	}
	
	while ( i < gaps.size() - 1 && gaps[i].end < gapped )
	{
		i++;
	}
	
	while ( i > 0 && gaps[i].start > gapped )
	{
		i--;
	}
	
	if ( gapped >= gaps[i].start )
	{
		if ( gapped <= gaps[i].end ) // within gap
		{
			pos.abs = gaps[i].start - 1;
			pos.gap = gapped - gaps[i].start + 1;
			
			if ( i > 0 )
			{
				pos.abs -= gaps[i - 1].offset;
			}
		}
		else // after gap
		{
			pos.abs = gapped - gaps[i].offset;
		}
	}
	else // before first gap
	{
		pos.abs = gapped;
	}
	
	return pos;
}

bool Alignment::loadPb(const Harvest::Alignment & msgAlignment, const Harvest::Variation & msgVariation, const Harvest::Reference & msgReference, int trackCount)
{
	totalLength = 0;
	filterFlags = 0;
	filterShow = false;
	
	lcbs.resize(msgAlignment.lcbs_size());
	tracks.resize(trackCount);
	
	for ( int i = 0; i < tracks.size(); i++ )
	{
		tracks[i] = new QVector<Region *>(msgAlignment.lcbs_size());
	}
	
	msgVariation.default_();
	
	for ( int i = 0; i < msgAlignment.lcbs_size(); i++ )
	{
		const Harvest::Alignment::Lcb & msgLcb = msgAlignment.lcbs(i);
		
		int id = i;
		
		lcbs[id].concordance = 1 - msgLcb.concordance();
		lcbs[id].number = atoi(msgLcb.name().c_str());
		
		QVector<Region *> * regions = new QVector<Region *>(msgLcb.regions_size());
		
		for ( int j = 0; j < msgLcb.regions_size(); j++ )
		{
			const Harvest::Alignment::Lcb::Region & msgRegion = msgLcb.regions(j);
			
			int track = j;
			
			char * snps = 0;
			
			Region * region = new Region
			(
			 id,
			 msgRegion.position(),
			 msgRegion.length(),
			 msgRegion.reverse(),
			 snps
			 );
			
			(*regions)[track] = region;
			(*tracks[track])[id] = region;
			
			if ( track == 0 )
			{
				totalLength += region->getLength();
			}
		}
		
		lcbs[id].regions = regions;
	}
	
	for ( int i = 0; i < tracks.size(); i++ )
	{
		qSort(tracks[i]->begin(), tracks[i]->end(), Region::lessThan);
		
		// determine total length
		//
		unsigned int length = 0;
		//
		for ( int j = 0; j < tracks[i]->size(); j++ )
		{
			length += (*tracks[i])[j]->getLength();
		}
		
		// assigned scaled
		//
		unsigned int position = 0;
		//
		for ( int j = 0; j < tracks[i]->size(); j++ )
		{
			(*tracks[i])[j]->setScaled
			(
			 (float)position / length,
			 (float)(position + (*tracks[i])[j]->getLength()) / length
			 );
			
			position += (*tracks[i])[j]->getLength();
		}
	}
	
	snpMap.clear();
	filters.resize(msgVariation.filters_size());
	
	for ( int i = 0; i < msgVariation.filters_size(); i++ )
	{
		const Harvest::Variation::Filter & msgFilter = msgVariation.filters(i);
		
		Filter & filter = filters[i];
		filter.flag = msgFilter.flag();
		filter.name = QString::fromStdString(msgFilter.name());
		filter.description = QString::fromStdString(msgFilter.description());
	}
	
	snpCount = 0;
	char refLast = 0;
	int posLast;
	gapsTotal = 0;
	Gap gap;
	int gapLength;
	snpPositions.resize(0);
	gaps.resize(0);
	snpsByPos.resize(0);
	
	for ( int i = 0; i < msgVariation.variants_size(); i++ )
	{
		const Harvest::Variation::Variant & msgSnp = msgVariation.variants(i);
		
		long int refOffset = 0;
		unsigned int refIndex = 0;
		
		int position = msgSnp.position();
		char charRef = msgSnp.alleles().c_str()[0];
		
		while ( msgSnp.sequence() > refIndex )
		{
			refOffset += msgReference.references(refIndex).sequence().length();
			refIndex++;
		}
		
		position += refOffset;
		
		if ( refLast == '-' && ( charRef != '-' || posLast != position ) )
		{
			gap.end = gap.startAbs + gapsTotal;
			gap.offset = gapsTotal;
			gaps.push_back(gap);
			//gapsTotal += gap.end - gap.start + 1;
		}
		
		unsigned int filters = msgSnp.filters();
		
		if ( charRef == '-' )
		{
			if ( refLast != '-' || posLast != position )
			{
				gap.start = position + gapsTotal + 1;
				gap.startAbs = position;
				gapLength = 0;
			}
			
			gapsTotal++;
			//gapLength++;
		}
		
		snpCount++;
		snpPositions.push_back(position + gapsTotal);
		snpsByPos.resize(snpsByPos.size() + 1);
		snpMap.insert(snpMap.end(), std::pair<long long int, int>(position + gapsTotal, snpsByPos.size() - 1));
		
		for ( unsigned int i = 0; i < msgSnp.alleles().length(); i++ )
		{
			char charQry = msgSnp.alleles().c_str()[i];
			
			if ( charQry != charRef )
			{
				Snp snp;
				snp.pos = i;
				snp.filters = filters;
				snp.snp = charQry;
				
				snpsByPos[snpsByPos.size() - 1].push_back(snp);
			}
		}
		
		posLast = position;
		refLast = charRef;
	}
	
	//totalLength = (*tracks[0])[tracks[0]->size() - 1]->getStart() + (*tracks[0])[tracks[0]->size() - 1]->getLength() + gapsTotal;
	
	QString refSeq;
	
	for ( int i = 0; i < msgReference.references_size(); i++ )
	{
		refSeq.append(QString::fromStdString(msgReference.references(i).sequence()));
	}
	
	int coreSize = 0;
	
	for ( int i = 0; i < lcbs.size(); i++ )
	{
		const Region * regionRef = (*lcbs[i].regions)[0];
		
		int startGapped = getPositionGapped(regionRef->getStart());
		int endGapped = getPositionGapped(regionRef->getStart() + regionRef->getLength() - 1);
		lcbs[i].startGapped = startGapped;
		lcbs[i].lengthGapped = endGapped - startGapped + 1;
		
		coreSize += regionRef->getLength();
	}
	
	core = (float)coreSize / refSeq.length();
	
	qSort(lcbs.begin(), lcbs.end(), lcbLessThan);
	
	totalLength = refSeq.length() + gapsTotal;
	
	refSeqCount = msgReference.references_size();
	refSeqStarts = new long long int[refSeqCount];
	
	long long int total = 0;
	
	for ( int i = 0; i < refSeqCount; i++ )
	{
		refSeqStarts[i] = getPositionGapped(total);
		total += msgReference.references(i).sequence().length();
	}
	
	refSeqGapped = new char[totalLength];
	int gapId = 0;
	gapsTotal = 0;
	
	for ( int i = 0; i < totalLength; i++ )
	{
		if ( i == gaps[gapId].start )
		{
			while( i <= gaps[gapId].end )
			{
				refSeqGapped[i] = '-';
				i++;
			}
			
			gapsTotal = gaps[gapId].offset;
			
			if ( gapId < gaps.size() - 1 )
			{
				gapId++;
			}
		}
		
		refSeqGapped[i] = refSeq.at(i - gapsTotal).toAscii(); // TODO: gap at pos 0?
	}
	
	setFilterScale();
	
	return true;
}
