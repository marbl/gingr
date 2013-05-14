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
	SnpData * newData,
	int newRadius
) :
alignment(newAlignment),
data(newData),
radius(newRadius)
{
}

SnpWorker::~SnpWorker()
{
}

void SnpWorker::process()
{
	computeLcbs();
	
	int bins = data->getBins();
	int start = data->getPosStart();
	int end = data->getPosEnd();
	
	int trackCount = alignment->getTracks()->size();
	float factor = float(bins - 1) / (end - start + 1); // TODO: last bin?
	int * snpsScale = new int[bins];
	int * snpSumScale = new int [bins];
	int * snpSumScaleSmooth = new int [bins];
//	int * snpSums = new int [bins];
	int * snpSums = data->getSnpSums();
	
	int snpMax = 0;
	int snpSumMax = 0;
	
	memset(snpSums, 0, bins * sizeof(int));
	memset(snpSumScale, 0, bins * sizeof(int));
//	memset(snpSumsSmooth, 0, bins * sizeof(int));
	memset(snpSumScaleSmooth, 0, bins * sizeof(int));
	
	for (int l = 0; l < 1; l++){
	for (int i = 0; i < trackCount; i++)
	{
		int firstSnp = alignment->getNextSnpIndex(i, start < 0 ? 0 : start);
		int * snps = data->getSnps()[i];
		
		memset(snps, 0, bins * sizeof(int));
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
				snps[bin] += lcbFactor * (radius + 1);//binWidth + binCenter - offset;
				
				snpSums[bin] += radius + 1;
				
				for ( int k = 1; k <= radius; k++ )
				{
					if ( bin >= k )
					{
						snps[bin - k] += lcbFactor * (radius + 1 - k);
						snpSums[bin - k] += lcbFactor * (radius + 1 - k);
					}
					
					if ( bin < bins - k )
					{
						snps[bin + k] += lcbFactor * (radius + 1 - k);
						snpSums[bin + k] += lcbFactor * (radius + 1 - k);
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
				
				snpSumScale[bin] += radius + 1;
				
				for ( int k = 1; k <= radius; k++ )
				{
					if ( bin >= k )
					{
						snpsScale[bin - k] += lcbFactor * (radius + 1 - k);
						snpSumScale[bin - k] += lcbFactor * (radius + 1 - k);
					}
					
					if ( bin < bins - k )
					{
						snpsScale[bin + k] += lcbFactor * (radius + 1 - k);
						snpSumScale[bin + k] += lcbFactor * (radius + 1 - k);
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
	/*
	for ( int i = 0; i < bins; i++ )
	{
		snpSumsSmooth[i] += snpSums[i] * (radius + 1);
		snpSumScaleSmooth[i] += snpSumScale[i] * (radius + 1);
		
		for ( int j = 1; j <= radius; j++ )
		{
			if ( i >= j )
			{
				snpSumsSmooth[i - j] += snpSums[i] * (radius + 1 - j);
				snpSumScaleSmooth[i - j] += snpSumScale[i] * (radius + 1 - j);
				//						snpSumScale[bin - k] += lcbFactor * (radius + 1 - k);
			}
			
			if ( i < bins - j )
			{
				snpSumsSmooth[i + j] += snpSums[i] * (radius + 1 - j);
				snpSumScaleSmooth[i + j] += snpSumScale[i] * (radius + 1 - j);
				//						snpSumScale[bin + k] += lcbFactor * (radius + 1 - k);
			}
		}
	}
	*/
	for ( int i = 0; i < bins; i++ )
	{
		if ( snpSumScale[i] > snpSumMax )
		{
			snpSumMax = snpSumScale[i];
		}
	}
	
	data->setSnpMax(snpMax);
	data->setSnpSumMax(snpSumMax);
	
	delete [] snpsScale;
    emit finished();
}

void SnpWorker::computeLcbs()
{
//	const RegionVector & refTracks = *((*alignment->getTracks())[0]);
	int bins = data->getBins();
	int start = data->getPosStart();
	int end = data->getPosEnd();
	int windowSize = end - start + 1;
	float binWidth = (float)bins / windowSize;
//	int gaps = 0;
	binMax = windowSize / bins;
	
	if ( binMax == 0 )
	{
		binMax = 1;
	}
	
	int * lcbs = data->getLcbs();
	
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

