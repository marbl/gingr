// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "Alignment.h"
#include <QObject>
#include <QStringList>

using namespace gav;

Alignment::Alignment()
{
	totalLength = 0;
	refSeqGapped = 0;
	refSeqStarts = 0;
	trackReference = 0;
	filterPass = true;
	filterPassScale = true;
	
}

Alignment::~Alignment()
{
	destroyRegions();
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
	
	while ( i > 0 && lcbs[i].startGapped + lcbs[i].lengthGapped - 1 > gapped )
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
	try{
	return snpMap.lower_bound(pos)->second;
	}catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
}

long long int Alignment::getPositionGapped(long long int ungapped) const
{
	try{
	if ( gaps.size() == 0 )
	{
		return ungapped;
	}
	
	int i = ungapped * (float)gaps.size() / (totalLength - gapsTotal);
	
	if ( i >= gaps.size() )
	{
		i = gaps.size() - 1;
	}
	
	while ( i < gaps.size() - 1 && gaps[i + 1].startAbs < ungapped )
	{
		i++;
	}
	
	while ( i > 0 && gaps[i].startAbs >= ungapped )
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
	}catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
}

Alignment::Position Alignment::getPositionUngapped(long long int gapped) const
{
	try{
	Position pos;
	
	pos.abs = gapped;
	pos.gap = 0;
	
	if ( gaps.size() == 0 )
	{
		return pos;
	}
	
	int i = gapped * (float)gaps.size() / totalLength;
	
	if ( i >= gaps.size() && gaps.size() )
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
	}catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
}

bool Alignment::init(const LcbList & lcbList, const VariantList & variantList, const ReferenceList & referenceList, const TrackList & trackList)
{
	totalLength = 0;
	filterFlags = 0;
	filterPass = true;
	filterPassScale = true;
	filterShow = false;
	trackReference = 0;
	destroyRegions();
	
	lcbs.resize(lcbList.getLcbCount());
	tracks.resize(trackList.getTrackCount());
	
	for ( int i = 0; i < tracks.size(); i++ )
	{
		tracks[i] = new QVector<Region *>(lcbList.getLcbCount());
	}
	
	for ( int i = 0; i < lcbList.getLcbCount(); i++ )
	{
		const LcbList::Lcb & lcb = lcbList.getLcb(i);
		
		int id = i;
		
		lcbs[id].concordance = 1 - lcb.concordance;
		//lcbs[id].number = atoi(lcb.name().c_str());
		
		QVector<Region *> * regions = new QVector<Region *>(lcb.regions.size());
		
		for ( int j = 0; j < lcb.regions.size(); j++ )
		{
			const LcbList::Region & lcbRegion = lcb.regions.at(j);
			
			int track = j;
			
			char * snps = 0;
			
			Region * region = new Region
			(
			 id,
			 lcbRegion.position,
			 lcbRegion.length,
			 lcbRegion.reverse,
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
	filters.resize(variantList.getFilterCount());
	
	for ( int i = 0; i < variantList.getFilterCount(); i++ )
	{
		const VariantList::Filter & msgFilter = variantList.getFilter(i);
		
		Filter & filter = filters[i];
		filter.flag = msgFilter.flag;
		filter.name = QString::fromStdString(msgFilter.name);
		filter.description = QString::fromStdString(msgFilter.description);
	}
	
	snpCount = 0;
	char refLast = 0;
	int posLast;
	gapsTotal = 0;
	Gap gap;
	int gapLength;
	snpColumns.resize(0);
	gaps.resize(0);
	
	for ( int i = 0; i < variantList.getVariantCount(); i++ )
	{
		const VariantList::Variant & msgSnp = variantList.getVariant(i);
		
		long int refOffset = 0;
		int refIndex = 0;
		
		int position = msgSnp.position;
		char charRef = msgSnp.alleles.c_str()[0];
		//char charRef = posLast == position ? '-' : referenceList.getReference(msgSnp.sequence).sequence[msgSnp.position];
		
		while ( msgSnp.sequence > refIndex ) // - 1 OLD )
		{
			refOffset += referenceList.getReference(refIndex).sequence.length();
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
		
		unsigned int filters = msgSnp.filters;
		
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
		snpColumns.resize(snpColumns.size() + 1);
		snpMap.insert(snpMap.end(), std::pair<long long int, int>(position + gapsTotal, snpColumns.size() - 1));
		
		SnpColumn & snpColumn = snpColumns[snpColumns.size() - 1];
		
		snpColumn.position = position + gapsTotal;
		
		// HACK for vcf; TODO: gapped coords in protobuf
		//
		snpColumn.ref = msgSnp.alleles.c_str()[0] == '-' ? '-' : referenceList.getReference(msgSnp.sequence).sequence[msgSnp.position];
		
		snpColumn.filters = filters;
		
		for ( unsigned int i = 0; i < msgSnp.alleles.length(); i++ )
		{
			char charQry = msgSnp.alleles.c_str()[i];
			
			if ( charQry != snpColumn.ref )
			{
				Snp snp;
				snp.track = i;
				snp.snp = charQry;
				
				snpColumn.snps.push_back(snp);
			}
		}
		
		posLast = position;
		refLast = charRef;
	}
	
	//totalLength = (*tracks[0])[tracks[0]->size() - 1]->getStart() + (*tracks[0])[tracks[0]->size() - 1]->getLength() + gapsTotal;
	
	QString refSeq;
	
	for ( int i = 0; i < referenceList.getReferenceCount(); i++ )
	{
		refSeq.append(QString::fromStdString(referenceList.getReference(i).sequence));
	}
	
	int coreSize = 0;
	
	for ( int i = 0; i < lcbs.size(); i++ )
	{
		const Region * regionRef = (*lcbs[i].regions)[0];
		
		int startGapped = getPositionGapped(regionRef->getStart() - 1) + 1;
		int endGapped = getPositionGapped(regionRef->getStart() + regionRef->getLength()) - 1;
		lcbs[i].startGapped = startGapped;
		lcbs[i].lengthGapped = endGapped - startGapped + 1;
		
		coreSize += regionRef->getLength();
	}
	
	core = (float)coreSize / refSeq.length();
	
	qSort(lcbs.begin(), lcbs.end(), lcbLessThan);
	
	totalLength = refSeq.length() + gapsTotal;
	
	refSeqCount = referenceList.getReferenceCount();
	
	if ( refSeqStarts )
	{
		delete [] refSeqStarts;
	}
	
	refSeqStarts = new long long int[refSeqCount];
	
	long long int total = 0;
	
	for ( int i = 0; i < refSeqCount; i++ )
	{
		refSeqStarts[i] = getPositionGapped(total);
		total += referenceList.getReference(i).sequence.length();
	}
	
	printf("total length %d\n", totalLength);
	
	if ( refSeqGapped )
	{
		delete [] refSeqGapped;
	}
	
	refSeqGapped = new char[totalLength];
	int gapId = 0;
	gapsTotal = 0;
	
	for ( int i = 0; i < totalLength; i++ )
	{
		if ( gaps.size() && i == gaps[gapId].start )
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
		
		if ( i - gapsTotal < refSeq.length() )
		{
			refSeqGapped[i] = refSeq.at(i - gapsTotal).toAscii(); // TODO: gap at pos 0?
		}
	}
	
	if ( trackList.getTrackReference() > 0 )
	{
		setTrackReference(trackList.getTrackReference());
	}
	
	setFilterScale();
	
	return true;
}

void Alignment::setTrackReference(int trackReferenceNew)
{
	trackReference = trackReferenceNew;
	
	for ( int i = 0; i < snpColumns.count(); i++ )
	{
		SnpColumn & snpColumn = snpColumns[i];
		
		snpColumn.ref = refSeqGapped[snpColumn.position];
		
		for ( int j = 0; j < snpColumn.snps.count(); j++ )
		{
			if ( snpColumn.snps[j].track == trackReference )
			{
				snpColumn.ref = snpColumn.snps[j].snp;
			}
		}
	}
}

void Alignment::destroyRegions()
{
	for ( int i = 0; i < tracks.size(); i++ )
	{
		delete tracks[i];
	}
	
	for ( int i = 0; i < lcbs.size(); i++ )
	{
		for ( int j = 0; j < lcbs[i].regions->size(); j++ )
		{
			delete lcbs[i].regions->at(j);
		}
		
		delete lcbs[i].regions;
	}
}

