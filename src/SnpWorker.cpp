//
//  SnpWorker.cpp
//  gavqt
//
//  Created by Brian Ondov on 1/8/13.
//
//

#include "SnpWorker.h"
#include <QThread>

SnpWorker::SnpWorker
(
	const Alignment * newAlignment,
	int ** newSnps,
	int * newLcbs,
	int newStart,
	int newEnd,
	int newBins,
	int newRadius,
	int & newSnpMax
) :
alignment(newAlignment),
snps(newSnps),
lcbs(newLcbs),
start(newStart),
end(newEnd),
bins(newBins),
radius(newRadius),
snpMax(newSnpMax)
{
}

SnpWorker::~SnpWorker()
{
}

void SnpWorker::process()
{
	computeLcbs();
	
	int trackCount = alignment->getTracks()->size();
	float factor = float(bins - 1) / (end - start + 1); // TODO: last bin?
	int * snpsScale = new int[bins];
	
	snpMax = 0;
	
	for (int l = 0; l < 1; l++){
	for (int i = 0; i < trackCount; i++)
	{
		int firstSnp = alignment->getNextSnpIndex(i, start < 0 ? 0 : start);
		
		memset(snps[i], 0, bins * sizeof(int));
		memset(snpsScale, 0, bins * sizeof(int));
		
		if ( alignment->getSnpCountByTrack(i) == 0 || alignment->getSnp(i, firstSnp).pos < start )
		{
			continue;
		}
		
		for
		(
			int j = firstSnp;
			
			j < alignment->getSnpCountByTrack(i) &&
			alignment->getSnp(i, j).pos <= end;
		 
			j++
		)
		{
			if ( alignment->filter(alignment->getSnp(i, j).filters) )
			{
				int offset = alignment->getSnp(i, j).pos - start;
				int bin = (int)(float(offset) * factor);
				int lcbFactor = 1;//(binMax - lcbs[bin]) * 200 / binMax + 1;
				
//				int radius = 5;
				snps[i][bin] += lcbFactor * (radius + 1);//binWidth + binCenter - offset;
				
				for ( int k = 1; k <= radius; k++ )
				{
					if ( bin >= k )
					{
						snps[i][bin - k] += lcbFactor * (radius + 1 - k);
					}
					
					if ( bin < bins - k )
					{
						snps[i][bin + k] += lcbFactor * (radius + 1 - k);
					}
				}
				/*
				if ( offset < binCenter )
				{
					if ( bin > 0 )
					{
						snps[i][bin - 1] += binCenter - offset;
					}
				}
				else
				{
					if ( bin < bins - 1 )
					{
						snps[i][bin + 1] += offset - binCenter;
					}
				}*/
			}
			
			if ( alignment->filterScale(alignment->getSnp(i, j).filters) )
			{
				int offset = alignment->getSnp(i, j).pos - start;
				int bin = (int)(float(offset) * factor);
				int lcbFactor = 1;//(binMax - lcbs[bin]) * 200 / binMax + 1;
				
				snpsScale[bin] += lcbFactor * (radius + 1);//binWidth + binCenter - offset;
				
				for ( int k = 1; k <= radius; k++ )
				{
					if ( bin >= k )
					{
						snpsScale[bin - k] += lcbFactor * (radius + 1 - k);
					}
					
					if ( bin < bins - k )
					{
						snpsScale[bin + k] += lcbFactor * (radius + 1 - k);
					}
				}
			}
		}
		
		for ( int j = 0; j < bins; j++ )
		{
//			snps[i][j] *= binMax - lcbs[j];
//			snpsScale[j] *= binMax - lcbs[j];
			
			if ( snpsScale[j] > snpMax )
			{
				snpMax = snpsScale[j];
			}
		}
	}
	}
	
	delete [] snpsScale;
    emit finished();
}

void SnpWorker::computeLcbs()
{
//	const RegionVector & refTracks = *((*alignment->getTracks())[0]);
	int windowSize = end - start + 1;
	float binWidth = (float)bins / windowSize;
//	int gaps = 0;
	binMax = windowSize / bins;
	
	if ( binMax == 0 )
	{
		binMax = 1;
	}
	
	memset(lcbs, 0, sizeof(int) * bins);
	
//	for ( int i = 0; i < refTracks.size(); i++ )
	for ( int i = alignment->getNextLcb(start); i < alignment->getLcbCount() && alignment->getLcb(i).startGapped <= end; i++ )
	{
		int posStartLcb = alignment->getLcb(i).startGapped;
//		int lcb = refTracks[i]->getLcb();
		
		if ( posStartLcb > end )
		{
			continue;
		}
		
		int posEndLcb = posStartLcb + alignment->getLcb(i).lengthGapped - 1;
		
		if ( posEndLcb < start )
		{
//			gaps += alignment->getLcb(lcb).lengthGapped - refTracks[i]->getLength();
			continue;
		}
		
		int binStart = posStartLcb < start ? 0 : (posStartLcb - start) * binWidth;
		int binEnd = posEndLcb > end ? bins - 1 : (posEndLcb - start) * binWidth;
		int posStartBin = binStart / binWidth + start;
		int posEndBin = (binEnd + 1) / binWidth - 1 + start;
		
		for ( int j = binStart; j <= binEnd; j++ )
		{
			lcbs[j] += binMax;
			
			if ( j == binStart )
			{
				lcbs[j] -= posStartLcb - posStartBin;
			}
			
			if ( j == binEnd )
			{
				lcbs[j] -= posEndBin - posEndLcb;
			}
		}
		
//		gaps += alignment->getLcb(lcb).lengthGapped - refTracks[i]->getLength();
	}
}

