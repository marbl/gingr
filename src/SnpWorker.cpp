// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "SnpWorker.h"
#include <QThread>
#include <QImage>
#include <QPainter>

SnpWorker::SnpWorker
(
	const Alignment * newAlignment,
	SnpData * newData,
	const std::vector<int> * idByTrackNew,
	QMutex * mutexNew,
	int newRadius,
	const SnpPalette * newPalette,
	const SyntenyPalette * newPaletteSynteny
) :
alignment(newAlignment),
data(newData),
idByTrack(idByTrackNew),
mutex(mutexNew),
radius(newRadius),
palette(newPalette),
paletteSynteny(newPaletteSynteny)
{
	trackById = new int[idByTrack->size()];
	
	for ( int i = 0; i < idByTrack->size(); i++ )
	{
		trackById[idByTrack->at(i)] = i;
	}
}

SnpWorker::~SnpWorker()
{
	delete [] trackById;
}

void SnpWorker::process()
{
	//mutex->lock();
	snpSumsSmooth = new int [data->getBins()];
	gapSumsSmooth = new int [data->getBins()];
	
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
	delete [] gapSumsSmooth;
	
    emit finished();
	//mutex->unlock();
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
		
		int posEndLcb = posStartLcb + alignment->getLcb(i).lengthGapped;
		
		if ( posEndLcb < start )
		{
//			gaps += alignment->getLcb(lcb).lengthGapped - refTracks[i]->getLength();
			continue;
		}
		
		int binStart = posStartLcb < start ? 0 : int((float)posStartLcb * binWidth) - int((float)start * binWidth);
		int binEnd = posEndLcb > end ? bins - 1 : int((float)posEndLcb * binWidth) - int((float)start * binWidth);
		
		for ( int j = binStart; j <= binEnd; j++ )
		{
			lcbs[j] += binMax;
			
			if ( j == binStart )
			{
				//lcbs[j] -= posStartLcb - posStartBin;
			}
			
			if ( j == binEnd )
			{
				//lcbs[j] -= posEndBin - posEndLcb;
			}
		}
		
//		gaps += alignment->getLcb(lcb).lengthGapped - refTracks[i]->getLength();
	}
}

void SnpWorker::computeSnps()
{
	bool showGaps = data->getShowGaps() & Alignment::SHOW;
	bool showIns = showGaps && data->getShowGaps() & Alignment::INSERTIONS;
	bool showDel = showGaps && data->getShowGaps() & Alignment::DELETIONS;
	
	int bins = data->getBins();
	int start = data->getPosStart();
	int end = data->getPosEnd();
	
	float factor = float(bins) / (end - start + 1); // TODO: last bin?
	
	int * snpSumsScaleSmooth = new int[bins];
	
	int trackCount = alignment->getTracks()->size();
	
	snpMaxScale = 0;
	snpMaxScaleView = 0;
	int snpMax = 0;
	int snpMaxView = 0;
	gapMax = 0;
	gapMaxView = 0;
	snpSumMaxScale = 0;
	gapSumMax = 0;
	float gapMean = 0;
	float gapMeanView = 0;
	occupied = 0;
	occupiedView = 0;
	occupiedSum = 0;
	
	memset(snpSumsSmooth, 0, bins * sizeof(int));
	memset(snpSumsScaleSmooth, 0, bins * sizeof(int));
	memset(gapSumsSmooth, 0, bins * sizeof(int));
	
	for (int i = 0; i < alignment->getTracks()->size(); i++)
	{
		memset(data->getSnps(i), 0, bins * sizeof(int));
		memset(data->getGaps(i), 0, bins * sizeof(int));
		memset(data->getSnpsScale(i), 0, bins * sizeof(int));
	}
	
	for ( int i = alignment->getNextSnpIndex(start); i < alignment->getSnpColumnCount() && alignment->getSnpColumn(i).position >= start && alignment->getSnpColumn(i).position <= end; i++ )
	{
		const Alignment::SnpColumn & snpColumn = alignment->getSnpColumn(i);
		
		int pos = snpColumn.position;
		char ref = alignment->getRefSeqGapped()[pos];
		char refSnp = snpColumn.ref;
		int bin = factor >= 1 ? (int)(float(pos - start) * factor) :
		(int)(float(pos) * factor) - (int)((float)start * factor);
		
		if ( showIns && refSnp == '-' && ref == '-' )
		{
			for ( int j = data->getTrackMin(); j <= data->getTrackMax(); j++ )
			{
				data->getGaps(idByTrack->at(j))[bin]++;
			}
		}
		
		if ( ref != refSnp )
		{
			// fill in the whole column as snps, and subtract non-snps later
			
			for ( int j = 0; j < trackCount; j++ )
			{
				int id;
				try{
					id = idByTrack->at(j);
				} catch (std::out_of_range & e) {
					std::cout << e.what();
				}
				
				if ( alignment->filter(snpColumn.filters, data->getFilters(), data->getFilterPass()) )
				{
					data->getSnps(id)[bin]++;
				}
				
				if ( alignment->filter(snpColumn.filters, data->getFiltersScale(), data->getFilterPassScale()) )
				{
					data->getSnpsScale(id)[bin]++;
				}
				
				if ( showDel && ref == '-' )
				{
					data->getGaps(id)[bin]++;
				}
			}
		}
		
		for ( int j = 0; j < snpColumn.snps.count(); j++ )
		{
			Alignment::Snp snp;
			
			try{
			snp = snpColumn.snps.at(j);
			} catch (std::out_of_range & e) {
				std::cout << e.what();
			}
			if ( trackById[snp.track] < data->getTrackMin() || trackById[snp.track] > data->getTrackMax() )
			{
				//continue;
			}
			
			int track = snp.track;
			
			int * snps = data->getSnps(track);
			int * gaps = data->getGaps(track);
			
			if ( alignment->filter(snpColumn.filters, data->getFilters(), data->getFilterPass()) )
			{
				if ( ref == refSnp )
				{
					snps[bin]++;
				}
				else
				{
					if ( snp.snp == refSnp )
					{
						snps[bin]--; // was filled in above, but was not a snp
					}
				}
			}
			
			if ( alignment->filter(snpColumn.filters, data->getFiltersScale(), data->getFilterPassScale()) )
			{
				if ( ref == refSnp )
				{
					data->getSnpsScale(track)[bin]++;
				}
				else
				{
					if ( snp.snp == refSnp )
					{
						data->getSnpsScale(track)[bin]--;
					}
				}
			}
			
			if ( showDel && refSnp != '-' )
			{
				if (snp.snp == '-' )
				{
					if ( ref != '-' )
					{
						gaps[bin]++;
					}
				}
				else if ( ref == '-' )
				{
					if ( snp.snp != '-' )
					{
						gaps[bin]--; // was filled in above, but was not deletion
					}
				}
			}
			
			if ( showIns && refSnp == '-' && ref == '-' )
			{
				gaps[bin]--; // was filled in above, but is not an insertion gap
			}
			
			if ( showIns && refSnp == '-' && ref != '-' && snp.snp == '-' )
			{
				gaps[bin]++;
			}
		}
	}
	
	bool viewFull = data->getTrackMin() == 0 && data->getTrackMax() == trackCount - 1;
	
	for (int i = 0; i < trackCount; i++)
	{
		bool inView =
			trackById[i] >= data->getTrackMin() &&
			trackById[i] <= data->getTrackMax();
		bool inViewPartial = inView && ! viewFull;
		
		//int id = idByTrack->at(i);
		int * snps = data->getSnps(i);
		int * gaps = data->getGaps(i);
		int * snpsScale = data->getSnpsScale(i);
		
		smooth(snps);
		smooth(snpsScale);
		
		if ( showGaps )
		{
			smooth(gaps);
		}
		
		if ( inView )
		{
			for ( int j = 0; j < bins; j++ )
			{
				snpSumsSmooth[j] += snps[j];
				snpSumsScaleSmooth[j] += snpsScale[j];
				gapSumsSmooth[j] += gaps[j];
			}
		}
		
		snpMax = max(snps, snpMax);
		snpMaxScale = max(snpsScale, snpMaxScale);
		
		if ( inViewPartial )
		{
			snpMaxView = max(snps, snpMaxView);
			snpMaxScaleView = max(snpsScale, snpMaxScaleView);
		}
		
		for ( int j = bins / 3; j < 2 * bins / 3; j++ )
		{
			if ( snps[j] )
			{
				occupied++;
				
				if ( inViewPartial )
				{
					occupiedView++;
				}
			}
		}
		
		if ( showGaps )
		{
			gapMax = max(gaps, gapMax);
			
			if ( inViewPartial )
			{
				gapMaxView = max(gaps, gapMaxView);
			}
			
			for ( int j = bins / 3; j < 2 * bins / 3; j++ )
			{
				gapMean += gaps[j];
				
				if ( inViewPartial )
				{
					gapMeanView += gaps[j];
				}
			}
		}
	}
	
	if ( viewFull )
	{
		snpMaxView = snpMax;
		snpMaxScaleView = snpMaxScale;
		gapMaxView = gapMax;
		gapMeanView = gapMean;
		occupiedView = occupied;
	}
	
	snpSumMaxScale = max(snpSumsScaleSmooth);
	
	if ( showGaps )
	{
		gapSumMax = max(gapSumsSmooth);
	}
	
	int lcb = 0;
	
	for ( int i = bins / 3; i < 2 * bins / 3; i++ )
	{
		if ( data->getLcbs()[i] > 0 )
		{
			lcb++;
		}
		
		if ( snpSumsSmooth[i] )
		{
			occupiedSum++;
		}
	}
	
	int trackCountView = data->getTrackMax() - data->getTrackMin() + 1;
	
	gapMean /= lcb * trackCount;
	gapMeanView /= lcb * trackCountView;
	occupied /= lcb * trackCount;
	occupiedView /= lcb * trackCountView;
	occupiedSum /= lcb;
	
	int gapLimit = gapMean * 10;
	int gapLimitView = gapMeanView * 10;
	
	data->setSnpMax(snpMax);
	data->setSnpMaxView(snpMaxView);
	gapMax = gapMax > gapLimit ? gapLimit : gapMax;
	gapMaxView = gapMaxView > gapLimitView ? gapLimitView : gapMaxView;
	//data->setGapMax(gapMax);
	
	delete [] snpSumsScaleSmooth;
}

void SnpWorker::drawSnps()
{
	float maxOffset;
	float maxOffsetView;
	
	if ( data->getLightColors() )
	{
		if ( snpMaxScale < 5 )
		{
			maxOffset = .05 * snpMaxScale + .1;
		}
		else
		{
			maxOffset = .3;
		}
		
		if ( snpMaxScaleView < 5 )
		{
			maxOffsetView = .05 * snpMaxScaleView + .1;
		}
		else
		{
			maxOffsetView = .3;
		}
	}
	else
	{
		if ( snpMaxScale < 5 )
		{
			maxOffset = .04 * snpMaxScale;
		}
		else
		{
			maxOffset = .2;
		}
		
		if ( snpMaxScaleView < 5 )
		{
			maxOffsetView = .04 * snpMaxScaleView;
		}
		else
		{
			maxOffsetView = .2;
		}
	}
	
	float offset = maxOffset - occupied / 2;
	
	if ( offset < 0.15 )//|| snpMaxScale < 3)
	{
		offset = 0.15;
	}
	
	float offsetView = maxOffsetView - occupiedView / 2;
	
	if ( offsetView < 0.15 )//|| snpMaxScale < 3)
	{
		offsetView = 0.15;
	}
	
	float paletteFactor = (float)(SnpPalette::PALETTE_SIZE - 1) / (snpMaxScale > 3 ? snpMaxScale : 4) * (1. - offset);
	float paletteFactorView = (float)(SnpPalette::PALETTE_SIZE - 1) / (snpMaxScaleView > 3 ? snpMaxScaleView : 4) * (1. - offsetView);
	
	//float gapFactor = (float)(GAP_RANGE) / (data->getGapMax() > 2 ? data->getGapMax() : 3);
	
	for ( int i = 0; i < alignment->getTracks()->size(); i++ )
	{
		bool inView = trackById[i] >= data->getTrackMin() && trackById[i] <= data->getTrackMax();
		drawSnps(data->getSnps(i), data->getGaps(i), data->getRow(i), (inView ? offsetView : offset) * SnpPalette::PALETTE_SIZE, inView ? paletteFactorView : paletteFactor, inView ? snpMaxScaleView : snpMaxScale, inView ? gapMaxView : gapMax);
		QPainter painter(data->getRowSmall(i));
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.drawImage(QRect(0, 0, data->getBins() / 2, 1), *data->getRow(i), QRect(0, 0, data->getBins(), 1));
	}
	
	float offsetSum = 0.3 - occupiedSum / 2;
	
	if ( offsetSum < 0 || snpSumMaxScale < 3)
	{
		offsetSum = 0;
	}
	
	float paletteFactorSum = (float)(SnpPalette::PALETTE_SIZE - 1) / (snpSumMaxScale > 3 ? snpSumMaxScale : 4) * (1. - offsetSum);
	
	//printf("max: %d\toffset: %f\tmaxSum: %d\tmaxOffset: %f\n", snpMaxScale, offset, snpSumMaxScale, offsetSum);
	
	drawSnps(snpSumsSmooth, gapSumsSmooth, data->getSum(), offsetSum * SnpPalette::PALETTE_SIZE, paletteFactorSum, snpSumMaxScale, gapSumMax);
}

void SnpWorker::drawSnps(int * snps, int * gaps, QImage * image, int paletteOffset, float paletteFactor, int max, int gapMax)
{
	int bins = data->getBins();
	
	for ( int i = 0; i < bins; i++ )
	{
		int count = snps ? snps[i] : 0;
		QRgb color;
		
		if ( data->getLcbs()[i] == 0 )
		{
			if ( data->getLightColors() )
			{
				color = qRgb(240, 240, 240);
			}
			else
			{
				color = qRgb(48, 48, 48);
			}
		}
		else
		{
			if ( data->getSnpMax() <= 1 || count == 0 )
			{
				color = palette->color(0);
			}
			else
			{
				if ( count > max )//|| (max < 2 && count > 0) )
				{
					color = palette->color(SnpPalette::PALETTE_SIZE - 1);
				}
				else
				{
					color = palette->color(paletteOffset + count * paletteFactor);
				}
			}
		}
		
		((QRgb *)image->scanLine(0))[i] = color;
		
		if ( gaps && data->getShowGaps() & Alignment::SHOW && gaps[i] > 0 )
		{
			int cyan = 160 + 64 * ((gaps[i] >= gapMax) ? 1. : (float)gaps[i] / gapMax);
			float alpha = (gaps[i] >= gapMax) ? 1. : (float)gaps[i] / gapMax;
			float alphaInv = 1. - alpha;
			QColor mix = QColor::fromRgb(((QRgb *)image->scanLine(0))[i]);
			((QRgb *)image->scanLine(0))[i] = qRgb(mix.red() * alphaInv, mix.green() * alphaInv + cyan * alpha, mix.blue() * alphaInv + cyan * alpha);
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
		
		if ( true || regions[0]->getStart() < data->getPosEnd() && regions[0]->getStart() + regions[0]->getLength() > data->getPosStart() )
		{
			int binStart = posStartLcb < start ? 0 : (posStartLcb - start) * binWidth;
			int binEnd = posEndLcb > end ? bins - 1 : (posEndLcb - start) * binWidth;
			
			float startScaledRef = regions[alignment->getTrackReference()]->getStartScaled();
			bool refRc = regions[alignment->getTrackReference()]->getRc();
			
			for ( int j = 0; j < alignment->getTracks()->size(); j++ )
			{
				float startScaled;
				float factor = (regions[j]->getEndScaled() - regions[j]->getStartScaled()) / (binEnd - binStart + 1);
				
				if ( regions[j]->getRc() != refRc )
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

int SnpWorker::max(int * row, int seed)
{
	int bins = data->getBins();
	int max = seed;
	
	for ( int i = bins / 3; i < 2 * bins / 3; i++ )
	{
		if ( row[i] > max )
		{
			max = row[i];
		}
	}
	
	return max;
}

void SnpWorker::smooth(int * src)
{
	int bins = data->getBins();
	int dst[bins];
	
	memset(dst, 0, bins * sizeof(int));
	
	for ( int i = 0; i < bins; i++ )
	{
		int bin = src[i];
		int radius;
		
		dst[i] += bin;
		
		if ( bin >= 16 )
		{
			radius = 3;
		}
		else if ( bin >= 8 )
		{
			radius = 2;
		}
		else if ( bin >= 2 )
		{
			radius = 1;
		}
		else
		{
			radius = 0;
		}
		
		for ( int j = 1; j <= radius; j++ )
		{
			if ( i >= j )
			{
				dst[i - j] += bin / (j + 1);
			}
			
			if ( i < bins - j )
			{
				dst[i + j] += bin / (j + 1);
			}
		}
	}
	
	memcpy(src, dst, bins * sizeof(int));
}
