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

SnpBuffer::~SnpBuffer()
{
	if ( snpsCur )
	{
		for ( int i = 0; i < trackCount; i++ )
		{
			delete [] snpsCur[i];
		}
		
		delete [] snpsCur;
	}
	
	if ( snpsNew )
	{
		for ( int i = 0; i < trackCount; i++ )
		{
			delete [] snpsNew[i];
		}
		
		delete [] snpsNew;
	}
	
	if ( lcbsCur )
	{
		delete [] lcbsCur;
	}
	
	if ( lcbsNew )
	{
		delete [] lcbsNew;
	}
}

void SnpBuffer::initialize(const Alignment *newAlignment)
{
	alignment = newAlignment;
	trackCount = alignment->getTracks()->size();
	snpsCur = 0;
	snpsNew = 0;
	binsCur = 0;
	binsNew = 0;
	lcbsNew = 0;
	lcbsCur = 0;
	updating = false;
	updateNeeded = false;
}

void SnpBuffer::update(int posStart, int posEnd, int bins)
{
	if ( updating )
	{
		if ( posStart != posStartNew || posEnd != posEndNew )
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
	
	if ( binsNew != bins )
	{
		// reallocate
		
		if ( snpsNew )
		{
			for ( int i = 0; i < trackCount; i++ )
			{
				delete [] snpsNew[i];
			}
			
			delete [] snpsNew;
		}
		
		snpsNew = new int * [trackCount];
		
		for (int i = 0; i < trackCount; i++)
		{
			snpsNew[i] = new int[bins];
		}
		
		if ( lcbsNew )
		{
			delete [] lcbsNew;
		}
		
		lcbsNew = new int[bins];
	}
	
	if ( posStart < 0 )
	{
//		posStart = 0;
	}
	
	posStartNew = posStart;
	posEndNew = posEnd;
	binsNew = bins;
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
		snpsNew,
		lcbsNew,
		posStart,
		posEnd,
		bins,
		radius,
		snpMaxNew
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

void SnpBuffer::swap()
{
	int ** snpsTemp = snpsCur;
	int * lcbsTemp = lcbsCur;
	int binsTemp = binsCur;
	
	snpsCur = snpsNew;
	lcbsCur = lcbsNew;
	binsCur = binsNew;
	
	snpsNew = snpsTemp;
	lcbsNew = lcbsTemp;
	binsNew = binsTemp;
	
	posStartCur = posStartNew;
	posEndCur = posEndNew;
	
	snpMaxCur = snpMaxNew;
}
