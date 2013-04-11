//
//  SnpBuffer.h
//  gavqt
//
//  Created by Brian Ondov on 1/13/13.
//
//

#ifndef __gavqt__SnpBuffer__
#define __gavqt__SnpBuffer__

#include <QObject>
#include "Alignment.h"

class SnpBuffer : public QObject
{
	Q_OBJECT

public:
	
	~SnpBuffer();
	int getBins() const;
	int * getLcbs() const;
	int getPosEnd() const;
	int getPosStart() const;
	int getSnpMax() const;
	int ** getSnps() const;
	void initialize(const Alignment * newAlignment);
	void update(int posStart, int posEnd, int bins);
	
public slots:
	
	void threadError(QString) {printf("Thread error.\n");}
	void updateFinished();
	
signals:
	
	void updated();
	
private:
	
	void swap();
	
	const Alignment * alignment;
	int ** snpsCur;
	int ** snpsNew;
	int * lcbsCur;
	int * lcbsNew;
	int binsCur;
	int binsNew;
	int binsQueue;
	int posStartCur;
	int posStartNew;
	int posStartQueue;
	int posEndCur;
	int posEndNew;
	int posEndQueue;
	int snpMaxCur;
	int snpMaxNew;
	int trackCount;
	
	bool updating;
	bool updateNeeded;
};

inline int SnpBuffer::getBins() const {return binsCur;}
inline int * SnpBuffer::getLcbs() const {return lcbsCur;}
inline int SnpBuffer::getPosEnd() const {return posEndCur;}
inline int SnpBuffer::getPosStart() const {return posStartCur;}
inline int ** SnpBuffer::getSnps() const {return snpsCur;}
inline int SnpBuffer::getSnpMax() const {return snpMaxCur;}
#endif /* defined(__gavqt__SnpBuffer__) */
