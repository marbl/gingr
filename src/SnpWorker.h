// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__SnpWorker__
#define __gavqt__SnpWorker__

#include <QObject>
#include <QMutex>
#include "Alignment.h"
#include "SnpData.h"
#include "SnpPalette.h"
#include "SyntenyPalette.h"
#include <vector>

#define GAP_RANGE 60

class SnpWorker : public QObject
{
    Q_OBJECT
	
public:
    SnpWorker
	(
		const Alignment * newAlignment,
		SnpData * newData,
		const std::vector<int> * idByTrackNew,
		QMutex * mutexNew,
		int newRadius,
		const SnpPalette * newPalette,
		const SyntenyPalette * newPaletteSynteny
	);
    ~SnpWorker();
	
	public slots:
    void process();
	
signals:
    void finished();
    void error(QString err);
	
private:
	
	void computeLcbs();
	void computeSnps();
	void drawSnps();
	void drawSnps(int * snps, int * gaps, QImage * image, int paletteOffset, float paletteFactor, int max, int gapMax);
	void drawSynteny();
	int max(int * row, int seed = 0);
	void smooth(int * src);
	
	const Alignment * alignment;
	SnpData * data;
	const std::vector<int> * idByTrack;
	QMutex * mutex;
	int * trackById;
	int * snpSumsSmooth;
	int * gapSumsSmooth;
	int radius;
	int binMax;
	const SnpPalette * palette;
	const SyntenyPalette * paletteSynteny;
	int snpMaxScale;
	int snpMaxScaleView;
	int snpSumMaxScale;
	int gapMax;
	int gapMaxView;
	int gapSumMax;
	float occupied;
	float occupiedView;
	float occupiedSum;
};


#endif /* defined(__gavqt__SnpWorker__) */
