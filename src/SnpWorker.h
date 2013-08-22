//
//  SnpWorker.h
//  gavqt
//
//  Created by Brian Ondov on 1/8/13.
//
//

#ifndef __gavqt__SnpWorker__
#define __gavqt__SnpWorker__

#include <QObject>
#include "Alignment.h"
#include "SnpData.h"
#include "SnpPalette.h"

class SnpWorker : public QObject
{
    Q_OBJECT
	
public:
    SnpWorker
	(
		const Alignment * newAlignment,
		SnpData * newData,
		int newRadius,
		const SnpPalette * newPalette
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
	void drawSnps(int * snps, QImage * image, float factor, int max);
	
	const Alignment * alignment;
	SnpData * data;
	int * snpSumsSmooth;
	int radius;
	int binMax;
	const SnpPalette * palette;
	int snpMax;
	int snpSumMax;
};


#endif /* defined(__gavqt__SnpWorker__) */
