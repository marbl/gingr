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

class SnpWorker : public QObject
{
    Q_OBJECT
	
public:
    SnpWorker
	(
		const Alignment * newAlignment,
		SnpData * newData,
		int newRadius
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
	SnpData * data;
	int radius;
	int binMax;
};


#endif /* defined(__gavqt__SnpWorker__) */
