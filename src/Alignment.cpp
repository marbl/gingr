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
	
}

Alignment::~Alignment()
{
	// TODO: delete all members of tracks and lcbs
	
	for ( int i = 0; i < tracks.size(); i++ )
	{
		delete snpsByTrack[i];
	}
	
	delete [] snpsByTrack;
}
/*
unsigned int Alignment::getNextSnpIndex(const Window & window) const
{
	unsigned int i = snpCounts[window.lcbStart] / 2;
	unsigned int delta = i / 2;
	
	while
	(
		(
			i > 0 &&
			snpPositions[window.lcbStart][i] > (unsigned int)window.posStart &&
			snpPositions[window.lcbStart][i - 1] > (unsigned int)window.posStart
		) ||
		(
			i < snpCounts[window.lcbStart] - 1 &&
			snpPositions[window.lcbStart][i] < (unsigned int)window.posStart
		)
	)
	{
		if ( snpPositions[window.lcbStart][i] > (unsigned int)window.posStart )
		{
			i -= delta;
		}
		else
		{
			i += delta;
		}
		
		if ( delta > 1 )
		{
			delta <<= 2;
		}
	}
	
	return i;
}
*/

bool Alignment::filter(int flags) const
{
//	return (bool)(flags & filterFlags) == filterShow;
	return (filterPass && flags == 0) || flags & filterFlags;
}

bool Alignment::filterScale(int flags) const
{
//	return (bool)(flags & filterFlagsScale) == filterShow;
	return (filterPassScale && flags == 0) || flags & filterFlagsScale;
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

int Alignment::getNextSnpIndex(int track, int pos) const
{
	int i = 0;
	int posCur;
	int posLast;
	int deltaLast = totalLength;
	QVector<Snp> & snps = *snpsByTrack[track];
	
//	if ( snpCountsByTrack[track] == 0 )
	if ( (*snpsByTrack[track]).size() == 0 )
	{
		return 0;
	}
	
	while
	(
		(
			i > 0 &&
			(posCur = snps[i].pos) > pos &&
			(posLast = snps[i - 1].pos) > pos
		) ||
		(
			i < snps.size() - 1 &&
			(posCur = snps[i].pos) < pos
		)
	)
	{
		int delta;
		
		if ( snps[i].pos > pos )
		{
			delta =
				snps.size() *
				(snps[i].pos - pos) /
				totalLength;
			
			if ( delta >= deltaLast )
			{
				delta = deltaLast - 1;
			}
			
			if ( delta == 0 )
			{
				delta = 1;
			}
			
			if ( delta > i )
			{
				i = 0;
			}
			else
			{
				i -= delta > 0 ? delta : 1;
			}
		}
		else
		{
			delta =
				getSnpCountByTrack(track) *
				(pos - snps[i].pos) /
				totalLength;
			
			if ( delta >= deltaLast )
			{
				delta = deltaLast - 1;
			}
			
			if ( delta == 0 )
			{
				delta = 1;
			}
			
			if ( i + delta >= getSnpCountByTrack(track) )
			{
				i = getSnpCountByTrack(track) - 1;
			}
			else
			{
				i += delta > 0 ? delta : 1;
			}
		}
		
		deltaLast = delta;
		
		if ( delta == 0 )
		{
			delta = snps[i].pos > pos ? -1 : 1;
		}
		
//		i += delta;
	}
	
	if ( snps[i].pos < pos )
	{
//		printf("uh oh\n");
	}
	
	if ( i >= snps.size() )
	{
		i = snps.size() - 1;
	}
	
	return i;
}

int Alignment::getNextSnpIndex(int pos) const
{
	int i = 0;
	int posCur;
	int posLast;
	int deltaLast = totalLength;
	
	//	if ( snpCountsByTrack[track] == 0 )
	if ( snpPositions.size() == 0 )
	{
		return 0;
	}
	
	while
		(
		 (
		  i > 0 &&
		  (posCur = snpPositions[i]) > pos &&
		  (posLast = snpPositions[i - 1]) > pos
		  ) ||
		 (
		  i < snpPositions.size() - 1 &&
		  (posCur = snpPositions[i]) < pos
		  )
		 )
	{
		int delta;
		
		if ( snpPositions[i] > pos )
		{
			delta =
			(float)snpCount *
			(snpPositions[i] - pos) /
			totalLength;
			
			if ( delta >= deltaLast )
			{
				delta = deltaLast - 1;
			}
			
			if ( delta == 0 )
			{
				delta = 1;
			}
			
			if ( delta > i )
			{
				i = 0;
			}
			else
			{
				i -= delta > 0 ? delta : 1;
			}
		}
		else
		{
			delta =
			(float)snpCount *
			(pos - snpPositions[i]) /
			totalLength;
			
			if ( delta >= deltaLast )
			{
				delta = deltaLast - 1;
			}
			
			if ( delta == 0 )
			{
				delta = 1;
			}
			
			if ( i + delta >= snpCount )
			{
				i = snpCount - 1;
			}
			else
			{
				i += delta > 0 ? delta : 1;
			}
		}
		
		deltaLast = delta;
		
		if ( delta == 0 )
		{
			delta = snpPositions[i] > pos ? -1 : 1;
		}
		
		//		i += delta;
	}
	
	if ( snpPositions[i] < pos )
	{
		printf("uh oh\n");
	}
	
	if ( i >= snpPositions.size() )
	{
		i = snpPositions.size() - 1;
	}
	
	return i;
}

int Alignment::getPositionGapped(int ungapped) const
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

Alignment::Position Alignment::getPositionUngapped(int gapped) const
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

bool Alignment::loadDom(const QDomElement *documentElement)
{
	totalLength = 0;
	filterFlags = 0;
	filterShow = false;
	
	QDomElement alignmentElement = documentElement->firstChildElement("lcbs");
	
	for
	(
		QDomElement elementLcb = alignmentElement.firstChildElement("lcb");
		! elementLcb.isNull();
		elementLcb = elementLcb.nextSiblingElement("lcb")
	)
	{
		int id = elementLcb.attribute("id").toInt();
		int snpCount = 0;
		/*
		int snpSize = snpPositions.size();
		
		if ( snpSize <= id )
		{
			snpPositions.resize(id + 1);
			snpCounts.resize(id + 1);
			for ( int i = snpSize; i <= id; i++ )
			{
				snpPositions[i] = 0;
				snpCounts[i] = 0;
			}
		}
		
		if ( elementLcb.hasAttribute("snps") && elementLcb.attribute("snps").length() > 0 )
		{
			QStringList snpPositionStrings = elementLcb.attribute("snps").split(",");
			snpCount = snpPositionStrings.length();
			
			snpPositions[id] = new unsigned int [snpCount];
			snpCounts[id] = snpCount;
			
			for ( int i = 0; i < snpCount; i++ )
			{
				snpPositions[id][i] = snpPositionStrings[i].toUInt() - 1;
				
				if ( snpPositionStrings[i].toUInt() == 0 )
				{
					printf("bad snp position, lcb %d, snp %d\n", id, i);
				}
			}
		}
		*/
		int sizeLcbs = lcbs.size();
		
		if ( sizeLcbs <= id )
		{
			lcbs.resize(id + 1);
		}
		
//		lcbs[id].lengthGapped = elementLcb.attribute("len").toInt();
		lcbs[id].concordance = 1 - elementLcb.attribute("con").toFloat();
		lcbs[id].number = elementLcb.attribute("name").toInt();
		
		QVector<Region *> * regions = new QVector<Region *>();
		
		for
		(
			QDomElement elementRegion = elementLcb.firstChildElement("rgn");
			! elementRegion.isNull();
			elementRegion = elementRegion.nextSiblingElement("rgn")
		)
		{
			int track = elementRegion.attribute("trk").toInt();
			
			int sizeTracks = tracks.size();
			
			if ( sizeTracks <= track )
			{
				tracks.resize(track + 1);
				
				for ( int i = sizeTracks; i <= track; i++ )
				{
					tracks[i] = new QVector<Region *>();
				}
			}
			
			int sizeLcb = regions->size();
			
			if ( sizeLcb <= track )
			{
				regions->resize(track + 1);
				
				for ( int i = sizeLcb; i < track; i++ )
				{
					(*regions)[i] = 0;
				}
			}
			
			char * snps = 0;
			
			Region * region = new Region
			(
				id,
				elementRegion.attribute("pos").toUInt(),
				elementRegion.attribute("len").toUInt(),
				elementRegion.attribute("str") != QObject::tr("+"),
				snps
			);
			
			(*regions)[track] = region;
			tracks[track]->push_back(region);
			
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
	
	//computeTrackSnps();
	/*
	unsigned int offset = 0;
	QDomElement gapsElement = documentElement->firstChildElement("gaps");
	
	for
	(
		QDomElement elementGap = gapsElement.firstChildElement("gap");
		! elementGap.isNull();
		elementGap = elementGap.nextSiblingElement("gap")
	)
	{
		gaps.resize(gaps.size() + 1);
		
		Gap * gap = &gaps[gaps.size() - 1];
		
		gap->start = elementGap.attribute("start").toInt();
		gap->end = elementGap.attribute("length").toInt() + gap->start - 1;
		gap->startAbs = gap->start - offset;
		offset += gap->end - gap->start + 1;
		gap->offset = offset;
	}
	
	totalLength += offset;
	gapsTotal = offset;
	*/
	
	snpsByTrack = new QVector<Snp> * [tracks.size()];
	
	for ( int i = 0; i < tracks.size(); i++ )
	{
		snpsByTrack[i] = new QVector<Snp>;
	}
	
	QDomElement snpsElement = documentElement->firstChildElement("snps");
	QDomElement filtersElement = snpsElement.firstChildElement("filters");
	
	for
	(
		QDomElement elementFilter = filtersElement.firstChildElement("filter");
		! elementFilter.isNull();
		elementFilter = elementFilter.nextSiblingElement("filter")
	)
	{
		filters.resize(filters.size() + 1);
		Filter & filter = filters[filters.size() - 1];
		filter.flag = elementFilter.attribute("flag").toInt();
		filter.name = elementFilter.firstChild().nodeValue();
		filter.description = elementFilter.attribute("desc");
	}
	
	snpCount = 0;
	char refLast = 0;
	int posLast;
	gapsTotal = 0;
	Gap gap;
	int gapLength;
	
	for
		(
		 QDomElement elementSnp = snpsElement.firstChildElement("snp");
		 true;
		 elementSnp = elementSnp.nextSiblingElement("snp")
		 )
	{
		int position;
		char charRef;
		
		if ( ! elementSnp.isNull() )
		{
			position = elementSnp.attribute("pos").toInt() - 1;
			charRef = elementSnp.attribute("col")[0].toAscii();
		}
		
		if ( refLast == '-' && ( elementSnp.isNull() || charRef != '-' || posLast != position ) )
		{
			gap.end = gap.startAbs + gapsTotal;
			gap.offset = gapsTotal;
			gaps.push_back(gap);
			//gapsTotal += gap.end - gap.start + 1;
		}
		
		if ( elementSnp.isNull() )
		{
			break;
		}
		
		unsigned int filters = elementSnp.attribute("filt").toInt();
		
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
		
		for ( int i = 1; i < elementSnp.attribute("col").length(); i++ )
		{
			char charQry = elementSnp.attribute("col")[i].toAscii();
			
			if
			(
				 charQry != charRef &&
			 true
//				 charQry != '-' &&
//				 charRef != '-' &&
//				 charQry != 'N' &&
//				 charRef != 'N' &&
//				 charQry != 'n' &&
//				 charRef != 'n'
			)
			{
				snpsByTrack[i]->resize(snpsByTrack[i]->size() + 1);
				
				Snp & snp = (*snpsByTrack[i])[snpsByTrack[i]->size() - 1];
				snp.pos = position + gapsTotal;//getPositionGapped(position);
				//snp.ref = charRef;
				snp.filters = filters;
				snp.snp = charQry;
				
				Snp snpByPos;
				snpByPos.pos = i;
				snpByPos.filters = filters;
				snpByPos.snp = charQry;
				
				snpsByPos[snpsByPos.size() - 1].push_back(snpByPos);
			}
		}
		
		posLast = position;
		refLast = charRef;
	}
	
	//totalLength = (*tracks[0])[tracks[0]->size() - 1]->getStart() + (*tracks[0])[tracks[0]->size() - 1]->getLength() + gapsTotal;
	
	for ( int i = 0; i < lcbs.size(); i++ )
	{
		const Region * regionRef = (*lcbs[i].regions)[0];
		
		int startGapped = getPositionGapped(regionRef->getStart());
		int endGapped = getPositionGapped(regionRef->getStart() + regionRef->getLength() - 1);
		lcbs[i].startGapped = startGapped;
		lcbs[i].lengthGapped = endGapped - startGapped + 1;
	}
	
	QDomElement seqsElement = documentElement->firstChildElement("seqs");
	QString refSeq;
	
	qSort(lcbs.begin(), lcbs.end(), lcbLessThan);
	
	for
		(
		 QDomElement elementSeq = seqsElement.firstChildElement("seq");
		 ! elementSeq.isNull();
		 elementSeq = elementSeq.nextSiblingElement("seq")
		 )
	{
		refSeq.append(elementSeq.firstChild().nodeValue());
	}
	
	totalLength = refSeq.length() + gapsTotal;
	
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
	
	//computeTrackSnps();
	/*
	 unsigned int offset = 0;
	 QDomElement gapsElement = documentElement->firstChildElement("gaps");
	 
	 for
	 (
	 QDomElement elementGap = gapsElement.firstChildElement("gap");
	 ! elementGap.isNull();
	 elementGap = elementGap.nextSiblingElement("gap")
	 )
	 {
	 gaps.resize(gaps.size() + 1);
	 
	 Gap * gap = &gaps[gaps.size() - 1];
	 
	 gap->start = elementGap.attribute("start").toInt();
	 gap->end = elementGap.attribute("length").toInt() + gap->start - 1;
	 gap->startAbs = gap->start - offset;
	 offset += gap->end - gap->start + 1;
	 gap->offset = offset;
	 }
	 
	 totalLength += offset;
	 gapsTotal = offset;
	 */
	
	snpsByTrack = new QVector<Snp> * [tracks.size()];
	
	for ( int i = 0; i < tracks.size(); i++ )
	{
		snpsByTrack[i] = new QVector<Snp>;
	}
	
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
	
	for ( int i = 0; i < msgVariation.variants_size(); i++ )
	{
		const Harvest::Variation::Variant & msgSnp = msgVariation.variants(i);
		
		int position = msgSnp.position();
		char charRef = msgSnp.alleles().c_str()[0];
		
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
		
		for ( unsigned int i = 1; i < msgSnp.alleles().length(); i++ )
		{
			char charQry = msgSnp.alleles().c_str()[i];
			
			if ( charQry != charRef )
			{
				snpsByTrack[i]->resize(snpsByTrack[i]->size() + 1);
				
				Snp & snp = (*snpsByTrack[i])[snpsByTrack[i]->size() - 1];
				snp.pos = position + gapsTotal;//getPositionGapped(position);
				//snp.ref = charRef;
				snp.filters = filters;
				snp.snp = charQry;
				
				Snp snpByPos;
				snpByPos.pos = i;
				snpByPos.filters = filters;
				snpByPos.snp = charQry;
				
				snpsByPos[snpsByPos.size() - 1].push_back(snpByPos);
			}
		}
		
		posLast = position;
		refLast = charRef;
	}
	
	//totalLength = (*tracks[0])[tracks[0]->size() - 1]->getStart() + (*tracks[0])[tracks[0]->size() - 1]->getLength() + gapsTotal;
	
	for ( int i = 0; i < lcbs.size(); i++ )
	{
		const Region * regionRef = (*lcbs[i].regions)[0];
		
		int startGapped = getPositionGapped(regionRef->getStart());
		int endGapped = getPositionGapped(regionRef->getStart() + regionRef->getLength() - 1);
		lcbs[i].startGapped = startGapped;
		lcbs[i].lengthGapped = endGapped - startGapped + 1;
	}
	
	qSort(lcbs.begin(), lcbs.end(), lcbLessThan);
	
	QString refSeq;
	
	for ( int i = 0; i < msgReference.references_size(); i++ )
	{
		refSeq.append(QString::fromStdString(msgReference.references(i).sequence()));
	}
	
	totalLength = refSeq.length() + gapsTotal;
	
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
/*
void Alignment::computeTrackSnps()
{
	printf("computeTrackSnps is obsolete\n");
	return;
	
	int trackCount = getTracks()->size();
	
//	snpsByTrack = new Snp * [trackCount];
	snpCountsByTrack = new unsigned int [trackCount];
	
	for (int i = 0; i < trackCount; i++)
	{
		unsigned int offset = 0;
		unsigned int snpCount = 0;
		
		// count
		//
		for ( int j = 0; j < lcbs.size(); j++ )
		{
			int lcb = (*tracks[0])[j]->getLcb();
			
			for ( unsigned int k = 0; k < snpCounts[lcb]; k++ )
			{
				char snpTrack = (*lcbs[lcb].regions)[i]->getSnp(k);
				char snpRef = (*lcbs[lcb].regions)[0]->getSnp(k);
				
				if
					(
					 snpTrack != snpRef &&
					 snpTrack != '-' &&
					 snpRef != '-' &&
					 snpTrack != 'N' &&
					 snpRef != 'N' &&
					 snpTrack != 'n' &&
					 snpRef != 'n'
					 )
				{
					snpCount++;
				}
			}
			
			snpCountsByTrack[i] = snpCount;
		}
		
		// allocate
		//
//		snpsByTrack[i] = new Snp [snpCountsByTrack[i]];
		
		// assign
		//
		snpCount = 0;
		unsigned int gaps = 0;
		//
		for ( int j = 0; j < lcbs.size(); j++ )
		{
			int lcb = (*tracks[0])[j]->getLcb();
			
			offset = (*tracks[0])[j]->getStart() + gaps;
			
			for ( unsigned int k = 0; k < snpCounts[lcb]; k++ )
			{
				char snpTrack = (*lcbs[lcb].regions)[i]->getSnp(k);
				char snpRef = (*lcbs[lcb].regions)[0]->getSnp(k);
				
				if
				(
					snpTrack != snpRef &&
					snpTrack != '-' &&
					snpRef != '-' &&
					snpTrack != 'N' &&
					snpRef != 'N' &&
					snpTrack != 'n' &&
					snpRef != 'n'
				)
				{
//					snpsByTrack[i][snpCount].pos = snpPositions[lcb][k] + offset;
//					snpsByTrack[i][snpCount].ref = (*tracks[0])[j]->getSnp(k);
//					snpsByTrack[i][snpCount].snp = (*tracks[i])[j]->getSnp(k);
					
//					if ( snpCount > 0 && snpsByTrack[i][snpCount].pos < snpsByTrack[i][snpCount - 1].pos )
					{
						printf("uh oh\n");
					}
					
					snpCount++;
				}
			}
			
			gaps += lcbs[lcb].lengthGapped - (*tracks[0])[j]->getLength();
			offset += (*tracks[0])[j]->getLength();
		}
	}
}
*/

