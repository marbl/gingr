//
//  SnpBuffer.cpp
//  gavqt
//
//  Created by Brian Ondov on 1/13/13.
//
//

#include "SnpBuffer.h"
#include "SnpWorker.h"
#include <QThread>
#include <string.h>
#include <QPainter>

SnpBuffer::SnpBuffer()
{
	alignment = 0;
	snpPaletteLight = new SnpPalette(true);
	snpPaletteDark = new SnpPalette(false);
	snpDataCur = 0;
	snpDataNew = 0;
}

SnpBuffer::~SnpBuffer()
{
	if ( snpDataNew )
	{
		delete snpDataNew;
	}
	
	if ( snpDataCur )
	{
		delete snpDataCur;
	}
	
	delete snpPaletteDark;
	delete snpPaletteLight;
}

void SnpBuffer::drawSnpSums(QImage *image, int top, int bottom, int posStart, int posEnd, int bins) const
{
	if ( ready() )
	{
		drawSnps(image, getSum(), top, bottom, posStart, posEnd, bins, getBins());
	}
}

void SnpBuffer::drawSnps(QImage *image, int row, int top, int bottom, int posStart, int posEnd, int bins) const
{
	if ( ready() )
	{
		int windowTarget = posEnd - posStart + 1;
		int windowSource = getPosEnd() - getPosStart() + 1;
		float binFactor = (float)windowSource / windowTarget * bins / getBins();
		
		if ( binFactor < 1 )
		{
			drawSnps(image, snpDataCur->getRowSmall(row), top, bottom, posStart, posEnd, bins, getBins() / 2);
		}
		else
		{
			drawSnps(image, getRow(row), top, bottom, posStart, posEnd, bins, getBins());
		}
	}
}

void SnpBuffer::initialize(const Alignment *newAlignment, const std::vector<int> * idByTrackNew)
{
	if ( snpDataNew )
	{
		delete snpDataNew;
	}
	
	if ( snpDataCur )
	{
		delete snpDataCur;
	}
	
	alignment = newAlignment;
	idByTrack = idByTrackNew;
	trackCount = alignment->getTracks()->size();
	snpDataNew = 0;
	snpDataCur = 0;
	updating = false;
	updateNeeded = false;
	wave = false;
	
}

void SnpBuffer::update(int posStart, int posEnd, int bins, int trackMin, int trackMax, bool synteny, bool light, int gaps)
{
	if ( ! alignment )
	{
		return;
	}
	
	if ( updating )
	{
		//if ( posStart != snpDataNew->getPosStart() || posEnd != snpDataNew->getPosEnd() )
		{
			//printf("queuing:\t%d\t[%d -\t%d]\t%s\n", bins, posStart, posEnd, synteny ? "syn" : "snp");
			posStartQueue = posStart;
			posEndQueue = posEnd;
			binsQueue = bins;
			trackMinQueue = trackMin;
			trackMaxQueue = trackMax;
			syntenyQueue = synteny;
			lightQueue = light;
			gapsQueue = gaps;
			updateNeeded = true;
		}
		
		return;
	}
	
	//printf("updating:\t%d\t[%d -\t%d]\t%s\n", bins, posStart, posEnd, synteny ? "syn" : "snp");
	
	if ( posEnd < 0 )
	{
		return;
	}
	
	if ( snpDataNew == 0 || snpDataNew->getBins() != bins )
	{
		// reallocate
		
		if ( snpDataNew != 0 )
		{
			delete snpDataNew;
		}
		
		snpDataNew = new SnpData(bins, trackCount);
	}
	
	snpDataNew->setWindow(posStart, posEnd);
	snpDataNew->setTracks(trackMin, trackMax);
	snpDataNew->setSynteny(synteny);
	snpDataNew->setLightColors(light);
	snpDataNew->setShowGaps(gaps);
	snpDataNew->setFilters
	(
		alignment->getFilters(),
		alignment->getFiltersScale(),
		alignment->getFilterPass(),
		alignment->getFilterPassScale()
	);
	
	int radius = (posEnd - posStart + 1) / bins / 6 - 1;
	
	if ( radius > 3 )
	{
		radius = 3;
	}
	
	if ( radius < 0 )
	{
		radius = 0;
	}
	
	bool async = true;
	
	QThread* thread = new QThread;
	SnpWorker* worker = new SnpWorker
	(
	 alignment,
	 snpDataNew,
	 idByTrack,
	 radius,
	 light ? snpPaletteLight : snpPaletteDark,
	 &syntenyPalette
	 );
	
	if ( async )
	{
		worker->moveToThread(thread);
		connect(worker, SIGNAL(error(QString)), this, SLOT(threadError(QString)));
		connect(thread, SIGNAL(started()), worker, SLOT(process()));
		connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
		connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
		connect(worker, SIGNAL(finished()), this, SLOT(updateFinished()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		thread->start();
		
		updating = true;
		updateNeeded = false;
	}
	else
	{
		worker->process();
		updating = true;
		updateNeeded = false;
		updateFinished();
		delete worker;
	}
}

void SnpBuffer::updateFinished()
{
	swap();
	
	//printf("finished:\t%d\t[%d -\t%d]\n", binsCur, posStartCur, posEndCur);
	updating = false;
	
	if ( updateNeeded )
	{
		update(posStartQueue, posEndQueue, binsQueue, trackMinQueue, trackMaxQueue, syntenyQueue, lightQueue, gapsQueue);
	}
	
	emit updated();
}

void SnpBuffer::drawSnps(QImage * image, QImage * snps, int top, int bottom, int posStart, int posEnd, int binsTarget, int binsSource) const
{
	QPainter painter(image);
	
	int windowTarget = posEnd - posStart + 1;
	int windowSource = getPosEnd() - getPosStart() + 1;
	
	float binFactor = (float)windowSource / windowTarget * binsTarget / binsSource;
	float binOffset = float(getPosStart() - posStart) * binsTarget / windowTarget;
	
	int startTarget = binOffset;
	int endTarget = (binsSource - 1) * binFactor + binOffset;
	int widthTarget = endTarget - startTarget + 1;
	
	int pmWidth;
	int pmOffset;
	
	if ( startTarget > 0 )
	{
		pmOffset = startTarget;
		startTarget = 0;
	}
	else
	{
		pmOffset = 0;
	}
	
	if ( endTarget >= binsTarget )
	{
		pmWidth = binsTarget - pmOffset;
	}
	else
	{
		pmWidth = endTarget - pmOffset + 1;
	}
	
	if ( pmWidth <= 0 )
	{
		return;
	}
	
	QPixmap pmSnps(pmWidth, 1);
	QPainter painterPixmap(&pmSnps);
	
	if ( false && binFactor < 1 )
	{
		painterPixmap.setRenderHint(QPainter::SmoothPixmapTransform);
	}
	
	painterPixmap.drawImage(QRect(startTarget, 0, widthTarget, 1), *snps, QRect(0, 0, binsSource, 1));
	painter.drawTiledPixmap(pmOffset, top, pmWidth, bottom - top + 1, pmSnps);
	int max = 0;
	
	return;
	
	int windowSize = posEnd - posStart + 1;
	int height = top - bottom + 1;
	
	int radius1;
	int radius2 = height / 2;
	
	if ( radius2 * 2 != height )
	{
		radius1 = radius2 + 1;
	}
	else
	{
		radius1 = radius2;
	}
	/*
	float binFactor = (float)windowSize /
	float(getPosEnd() - getPosStart()) *
	binsSource /
	bins;
	
	float binOffset = float(posStart - getPosStart()) *
	binsSource /
	(getPosEnd() - getPosStart());
	*/
	float paletteFactor = (float)SnpPalette::PALETTE_SIZE / max;
	
	for ( int j = 0; j < binsTarget; j++ )
	{break;
		int bin = j * binFactor + binOffset;
		
		if ( bin < 0 || bin >= binsSource )
		{
			continue;
		}
		/*
		if ( bin < 0 )
		{
			snpsCur = &snpsLeft;
		}
		else if ( bin > snpsCenter->binsSource )
		{
			snpsCur = &snpsRight;
		}
		else
		{
			snpsCur = snpsCenter;
		}
		*/
		
		int count = 0;//getSnpMax() > 0 ? snps[bin] : 0;
		
		if ( wave )
		{
			float breakFactor = (float)(getLcbs()[bin] * binsSource) / windowSize;
			int breakShade = 220;
			int shade = breakShade + 35 * breakFactor;
			
			for ( int k = 0; k + radius2 < height; k++ )
			{
				((QRgb *)image->scanLine(top + radius2 + k))[j] = qRgb(shade, shade, shade);
			}
			int peak = count * (radius1) / max;
			int remainder = count - peak * max / radius1;
			
			if ( peak > height )
			{
				peak = height;
			}
			
			//for ( int k = (height - peak) / 2; k < peak + (height - peak) / 2 && k + getTrackHeight(i) < image.height(); k++ )
			for ( int k = 0; k <= peak && k + radius2 < height; k++ )
			{
				if ( k == peak )
				{
					int shade = 255 - remainder * radius1 * 255 / max;
					int gb = shade * (220 + 35 * (breakFactor)) / 255;
					
					((QRgb *)image->scanLine(top + radius2 + k))[j] = qRgb(128 + shade / 2 * (93 + 35 * (1. - breakFactor)) / 128, gb, gb);
				}
				else
				{
					((QRgb *)image->scanLine(top + radius2 + k))[j] = qRgb(128, 0, 0);
				}
			}
		}
		else
		{
			int shade;// = max ? 255 - count * 256 / max : 255;
			//float breakFactor = 1. - (float)(getLcbs()[j] * binsSource) / windowSize;
			//int breakShade = 220;
			//int gb = shade * (220 + 35 * (1. - breakFactor)) / 255;
			//((QRgb *)image.scanLine(getTrackHeight(i)))[j] = qRgb((128 + shade / 2) * (1. - breakFactor) + breakShade * breakFactor, shade * (1. - breakFactor) + breakShade * breakFactor, shade * (1. - breakFactor) + breakShade * breakFactor);
			//((QRgb *)image.scanLine(getTrackHeight(i)))[j] = qRgb(128 + shade / 2 * (93 + 35 * (1. - breakFactor)) / 128, gb, gb);
			//shade = j;
			//shade %= 256;
			
			shade = max > 2 ? count * paletteFactor : SnpPalette::PALETTE_SIZE - 1;
			
			if ( shade >= SnpPalette::PALETTE_SIZE )
			{
				shade = SnpPalette::PALETTE_SIZE - 1;
			}
			if ( shade < 0 )
			{
				shade = 0;
			}
			
			if ( getLcbs()[bin] == 0 )
			{
				((QRgb *)image->scanLine(top))[j] = qRgb(80, 80, 80);
			}
			else
			{
				//((QRgb *)image->scanLine(top))[j] = snpPalette.color(shade);
			}
		}
	}
	
	if ( wave )
	{
		for ( int j = 0; j < radius2; j++ )
		{
			memcpy(image->scanLine(top + j), image->scanLine(top - j), sizeof(char) * 4 * binsTarget);
		}
	}
	else
	{
		for ( int j = top + 1; j <= bottom; j++ )
		{
			memcpy(image->scanLine(j), image->scanLine(top), sizeof(char) * 4 * binsTarget);
		}
	}
}

void SnpBuffer::swap()
{
	SnpData * temp = snpDataCur;
	snpDataCur = snpDataNew;
	snpDataNew = temp;
}
