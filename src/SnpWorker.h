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

class SnpWorker : public QObject
{
    Q_OBJECT
	
public:
    SnpWorker
	(
		const Alignment * newAlignment,
		int ** newSnps,
		int * newLcbs,
		int newStart,
		int newEnd,
		int newBins,
		int newRadius,
		int & newSnpMax
	);
    ~SnpWorker();
	
	public slots:
    void process();
	
signals:
    void finished();
    void error(QString err);
	
private:
	
	void computeLcbs();
	
	const Alignment * alignment;
	int ** snps;
	int * lcbs;
	int start;
	int end;
	int bins;
	int radius;
	int & snpMax;
	int binMax;
};


#endif /* defined(__gavqt__SnpWorker__) */
