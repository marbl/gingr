//
//  SnpWorker.cpp
//  gavqt
//
//  Created by Brian Ondov on 1/8/13.
//
//

#include "SnpWorker.h"
#include <QThread>
#include <QImage>
#include <QPainter>

SnpWorker::SnpWorker
(
	const Alignment * newAlignment,
	SnpData * newData,
	int newRadius,
	const SnpPalette * newPalette,
	const SyntenyPalette * newPaletteSynteny
) :
alignment(newAlignment),
data(newData),
radius(newRadius),
palette(newPalette),
paletteSynteny(newPaletteSynteny)
{
}

SnpWorker::~SnpWorker()
{
}

void SnpWorker::process()
{
	snpSumsSmooth = new int [data->getBins()];
	
	computeLcbs();
	
	if ( data->getSynteny() )
	{
		drawSynteny();
	}
	else
	{
		computeSnps();
		drawSnps();
	}
	
	delete [] snpSumsSmooth;
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

void SnpWorker::computeSnps()
{
	int bins = data->getBins();
	int start = data->getPosStart();
	int end = data->getPosEnd();
	
	int trackCount = alignment->getTracks()->size();
	float factor = float(bins - 1) / (end - start + 1); // TODO: last bin?
	
	int * snps = new int[bins];
	int * snpsScale = new int[bins];
	int * snpsScaleSmooth = new int[bins];
	int * snpSumsScaleSmooth = new int[bins];
	
	snpMax = 0;
	snpSumMax = 0;
	
	memset(snpSumsSmooth, 0, bins * sizeof(int));
	memset(snpSumsScaleSmooth, 0, bins * sizeof(int));
	
	for (int l = 0; l < 1; l++){
		for (int i = 0; i < trackCount; i++)
		{
			int firstSnp = alignment->getNextSnpIndex(i, start < 0 ? 0 : start);
			int * snpsSmooth = data->getSnps(i);
			
			memset(snps, 0, bins * sizeof(int));
			memset(snpsScale, 0, bins * sizeof(int));
			memset(snpsSmooth, 0, bins * sizeof(int));
			memset(snpsScaleSmooth, 0, bins * sizeof(int));
			
			if ( alignment->getSnpCountByTrack(i) == 0 || alignment->getSnp(i, firstSnp).pos < start )
			{
				continue;
			}
			
			const Alignment::Snp * snp;
			int snpCount = alignment->getSnpCountByTrack(i);
			
			for
				(
				 int j = firstSnp;
				 
				 j < snpCount &&
				 (snp = &alignment->getSnp(i, j))->pos <= end;
				 
				 j++
				 )
			{
				if ( alignment->filter(snp->filters, data->getFilters(), data->getFilterPass()) )
				{
					int offset = snp->pos - start;
					int bin = (int)(float(offset) * factor);
					//				int lcbFactor = 1;//(binMax - lcbs[bin]) * 200 / binMax + 1;
					
					snps[bin]++;
				}
				
				if ( alignment->filter(snp->filters, data->getFiltersScale(), data->getFilterPassScale()) )
				{
					int offset = snp->pos - start;
					int bin = (int)(float(offset) * factor);
					
					snpsScale[bin]++;
				}
			}
			
			for ( int j = 0; j < bins; j++ )
			{
				int snpsBin = snps[j];
				int snpsScaleBin = snpsScale[j];
				
				snpsSmooth[j] += snpsBin * (radius + 1);
				snpsScaleSmooth[j] += snpsScaleBin * (radius + 1);
				
				for ( int k = 1; k <= radius; k++ )
				{
					if ( j >= k )
					{
						snpsSmooth[j - k] += snpsBin * (radius + 1 - k);
						snpsScaleSmooth[j - k] += snpsScaleBin * (radius + 1 - k);
					}
					
					if ( j < bins - k )
					{
						snpsSmooth[j + k] += snpsBin * (radius + 1 - k);
						snpsScaleSmooth[j + k] += snpsScaleBin * (radius + 1 - k);
					}
				}
			}
			
			for ( int j = 0; j < bins; j++ )
			{
				//			snps[i][j] *= binMax - lcbs[j];
				//			snpsScale[j] *= binMax - lcbs[j];
				
				snpSumsSmooth[j] += snpsSmooth[j];
				snpSumsScaleSmooth[j] += snpsScaleSmooth[j];
				
				if ( snpsScaleSmooth[j] > snpMax )
				{
					snpMax = snpsScaleSmooth[j];
				}
			}
		}
	}
	
	for ( int i = 0; i < bins; i++ )
	{
		if ( snpSumsScaleSmooth[i] > snpSumMax )
		{
			snpSumMax = snpSumsScaleSmooth[i];
		}
	}
	
	delete [] snps;
	delete [] snpsScale;
	delete [] snpsScaleSmooth;
	delete [] snpSumsScaleSmooth;
}

void SnpWorker::drawSnps()
{
	float paletteFactor = (float)(SnpPalette::PALETTE_SIZE - 1) / snpMax;
	
	for ( int i = 0; i < alignment->getTracks()->size(); i++ )
	{
		drawSnps(data->getSnps(i), data->getRow(i), paletteFactor, snpMax);
		QPainter painter(data->getRowSmall(i));
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.drawImage(QRect(0, 0, data->getBins() / 2, 1), *data->getRow(i), QRect(0, 0, data->getBins(), 1));
	}
	
	drawSnps(snpSumsSmooth, data->getSum(), (float)(SnpPalette::PALETTE_SIZE - 1) / snpSumMax, snpSumMax);
}

void SnpWorker::drawSnps(int * snps, QImage * image, float factor, int max)
{
	int bins = data->getBins();
	
	for ( int i = 0; i < bins; i++ )
	{
		int count = snps[i];
		int shade;
		
		if ( snpMax == 0 )
		{
			shade = 0;
		}
		else if ( count > max || (max < 3 && count > 0) )
		{
			shade = SnpPalette::PALETTE_SIZE - 1;
		}
		else
		{
			shade = count * factor;
		}
		
		if ( data->getLcbs()[i] == 0 )
		{
			((QRgb *)image->scanLine(0))[i] = qRgb(80, 80, 80);
		}
		else
		{
			((QRgb *)image->scanLine(0))[i] = palette->color(shade);
		}
	}
}

void SnpWorker::drawSynteny()
{
	int bins = data->getBins();
	int start = data->getPosStart();
	int end = data->getPosEnd();
	int windowSize = end - start + 1;
	float binWidth = (float)bins / windowSize;
	
	for ( int i = 0; i < alignment->getLcbCount(); i++ )
	{
		const RegionVector & regions = *alignment->getLcb(i).regions;
		
		int posStartLcb = alignment->getLcb(i).startGapped;
		
		if ( posStartLcb > end )
		{
			continue;
		}
		
		int posEndLcb = posStartLcb + alignment->getLcb(i).lengthGapped - 1;
		
		if ( posEndLcb < start )
		{
			continue;
		}
		
		if ( regions[0]->getStart() + regions[0]->getLength() > data->getPosStart() && regions[0]->getStart() < data->getPosEnd() )
		{
			int binStart = posStartLcb < start ? 0 : (posStartLcb - start) * binWidth;
			int binEnd = posEndLcb > end ? bins - 1 : (posEndLcb - start) * binWidth;
			
			for ( int j = 0; j < alignment->getTracks()->size(); j++ )
			{
				float startScaled;
				float factor = (regions[j]->getEndScaled() - regions[j]->getStartScaled()) / (binEnd - binStart + 1);
				
				if ( regions[j]->getRc() )
				{
					startScaled = regions[j]->getEndScaled();
					factor = -factor;
				}
				else
				{
					startScaled = regions[j]->getStartScaled();
				}
				
				QRgb * scanLine = ((QRgb *)data->getRow(j)->scanLine(0));
				
				for ( int k = (binStart < 0 ? 0 : binStart); k <= (binEnd >= bins ? bins - 1 : binEnd); k++ )
				{
					int shade = (startScaled + factor * (k - binStart)) * SyntenyPalette::PALETTE_SIZE;
					
					scanLine[k] = paletteSynteny->color(shade);
				}
			}
		}
	}
	
	for ( int i = 0; i < alignment->getTracks()->size(); i++ )
	{
		for ( int j = 0; j < bins; j++ )
		{
			if ( data->getLcbs()[j] == 0 )
			{
				((QRgb *)data->getRow(i)->scanLine(0))[j] = qRgb(100, 100, 100);
			}
		}
		
		QPainter painter(data->getRowSmall(i));
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.drawImage(QRect(0, 0, data->getBins() / 2, 1), *data->getRow(i), QRect(0, 0, data->getBins(), 1));
	}
	
	memcpy(data->getSum()->scanLine(0), data->getRow(0)->scanLine(0), data->getBins() * sizeof(QRgb));
}
