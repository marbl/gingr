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
#include <QtCore/qmath.h>

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
}

void SnpBuffer::drawSnpSums(QImage *image, int top, int bottom, int posStart, int posEnd, int bins) const
{
	if ( ready() )
	{
		drawSnps(image, getSnpSums(), getSnpSumMax(), top, bottom, posStart, posEnd, bins);
	}
}

void SnpBuffer::drawSnps(QImage *image, int row, int top, int bottom, int posStart, int posEnd, int bins) const
{
	if ( ready() )
	{
		drawSnps(image, getSnps()[row], getSnpMax(), top, bottom, posStart, posEnd, bins);
	}
}

void SnpBuffer::initialize(const Alignment *newAlignment)
{
	alignment = newAlignment;
	trackCount = alignment->getTracks()->size();
	snpDataNew = 0;
	snpDataCur = 0;
	updating = false;
	updateNeeded = false;
	wave = false;
	
	for ( int i = 0; i < PALETTE_SIZE; i++ )
	{
		float x = (float)i / PALETTE_SIZE;
		
		// gnuplot default heatmap colors (rgbformulae 7,5,15)
		//
		int r = 256 * qSqrt(x);
		int g = 256 * qPow(x, 3);
		int b = 256 * qSin(2 * 3.1415926 * x);
		
		if ( b < 0 )
		{
			b = 0;
		}
		
		snpPalette[i] = qRgb(r, g, b);
	}
	
	snpPalette[PALETTE_SIZE - 1] = qRgb(255, 255, 255);
}

void SnpBuffer::update(int posStart, int posEnd, int bins)
{
	if ( updating )
	{
		if ( posStart != snpDataNew->getPosStart() || posEnd != snpDataNew->getPosEnd() )
		{
			posStartQueue = posStart;
			posEndQueue = posEnd;
			binsQueue = bins;
			updateNeeded = true;
		}
		
		return;
	}
	
	//printf("updating:\t%d\t[%d -\t%d]\n", bins, posStart, posEnd);
	
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
	
	int radius = (posEnd - posStart + 1) / bins / 6 - 1;
	
	if ( radius > 3 )
	{
		radius = 3;
	}
	
	if ( radius < 0 )
	{
		radius = 0;
	}
	
	QThread* thread = new QThread;
	SnpWorker* worker = new SnpWorker
	(
	 alignment,
	 snpDataNew,
	 radius
	 );
	
	//worker->process();
	
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
	
	//updateFinished();
	//delete worker;
}

void SnpBuffer::updateFinished()
{
	swap();
	
	//printf("finished:\t%d\t[%d -\t%d]\n", binsCur, posStartCur, posEndCur);
	updating = false;
	
	if ( updateNeeded )
	{
		update(posStartQueue, posEndQueue, binsQueue);
	}
	
	emit updated();
}

void SnpBuffer::drawSnps(QImage * image, const int * snps, int max, int top, int bottom, int posStart, int posEnd, int bins) const
{
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
	
	float binFactor = (float)windowSize /
	float(getPosEnd() - getPosStart()) *
	getBins() /
	bins;
	
	float binOffset = float(posStart - getPosStart()) *
	getBins() /
	(getPosEnd() - getPosStart());
	
	float paletteFactor = (float)PALETTE_SIZE / max;
	
	for ( int j = 0; j < bins; j++ )
	{
		int bin = j * binFactor + binOffset;
		
		if ( bin < 0 || bin >= getBins() )
		{
			continue;
		}
		/*
		if ( bin < 0 )
		{
			snpsCur = &snpsLeft;
		}
		else if ( bin > snpsCenter->getBins() )
		{
			snpsCur = &snpsRight;
		}
		else
		{
			snpsCur = snpsCenter;
		}
		*/
		
		int count = getSnpMax() > 0 ? snps[bin] : 0;
		
		if ( wave )
		{
			float breakFactor = (float)(getLcbs()[bin] * getBins()) / windowSize;
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
					int shade = 255 - remainder * radius1 * 255 / getSnpMax();
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
			//float breakFactor = 1. - (float)(getLcbs()[j] * getBins()) / windowSize;
			//int breakShade = 220;
			//int gb = shade * (220 + 35 * (1. - breakFactor)) / 255;
			//((QRgb *)image.scanLine(getTrackHeight(i)))[j] = qRgb((128 + shade / 2) * (1. - breakFactor) + breakShade * breakFactor, shade * (1. - breakFactor) + breakShade * breakFactor, shade * (1. - breakFactor) + breakShade * breakFactor);
			//((QRgb *)image.scanLine(getTrackHeight(i)))[j] = qRgb(128 + shade / 2 * (93 + 35 * (1. - breakFactor)) / 128, gb, gb);
			//shade = j;
			//shade %= 256;
			
			shade = max ? count * paletteFactor : PALETTE_SIZE - 1;
			
			if ( shade >= PALETTE_SIZE )
			{
				shade = PALETTE_SIZE - 1;
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
				((QRgb *)image->scanLine(top))[j] = snpPalette[shade];
			}
		}
	}
	
	if ( wave )
	{
		for ( int j = 0; j < radius2; j++ )
		{
			memcpy(image->scanLine(top + j), image->scanLine(top - j), sizeof(char) * 4 * bins);
		}
	}
	else
	{
		for ( int j = top + 1; j <= bottom; j++ )
		{
			memcpy(image->scanLine(j), image->scanLine(top), sizeof(char) * 4 * bins);
		}
	}
}

void SnpBuffer::swap()
{
	SnpData * temp = snpDataCur;
	snpDataCur = snpDataNew;
	snpDataNew = temp;
}
