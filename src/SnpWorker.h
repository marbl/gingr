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
#include "SyntenyPalette.h"

class SnpWorker : public QObject
{
    Q_OBJECT
	
public:
    SnpWorker
	(
		const Alignment * newAlignment,
		SnpData * newData,
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
	void drawSnps(int * snps, QImage * image, float factor, int max);
	void drawSynteny();
	
	const Alignment * alignment;
	SnpData * data;
	int * snpSumsSmooth;
	int radius;
	int binMax;
	const SnpPalette * palette;
	const SyntenyPalette * paletteSynteny;
	int snpMaxScale;
	int snpSumMaxScale;
};


#endif /* defined(__gavqt__SnpWorker__) */
