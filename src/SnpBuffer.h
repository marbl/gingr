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
#include "SnpData.h"
#include <QImage>

const int PALETTE_SIZE = 1 << 10;

class SnpBuffer : public QObject
{
	Q_OBJECT

public:
	
	~SnpBuffer();
	
	void drawSnpSums(QImage * image, int top, int bottom, int posStart, int posEnd, int bins) const;
	void drawSnps(QImage * image, int row, int top, int bottom, int posStart, int posEnd, int bins) const;
	int getBins() const;
	int * getLcbs() const;
	int getPosEnd() const;
	int getPosStart() const;
	int getSnpMax() const;
	int ** getSnps() const;
	int * getSnpSums() const;
	int getSnpSumMax() const;
	void initialize(const Alignment * newAlignment);
	bool ready() const;
	void update(int posStart, int posEnd, int bins);
	
public slots:
	
	void threadError(QString) {printf("Thread error.\n");}
	void updateFinished();
	
signals:
	
	void updated();
	
private:
	
	void drawSnps(QImage * image, const int * snps, int max, int top, int bottom, int posStart, int posEnd, int bins) const;
	void swap();
	
	QRgb snpPalette[PALETTE_SIZE];
	const Alignment * alignment;
	SnpData * snpDataNew;
	SnpData * snpDataCur;
	int binsQueue;
	int posStartQueue;
	int posEndQueue;
	int trackCount;
	
	bool wave; // TEMP
	bool updating;
	bool updateNeeded;
};

inline int SnpBuffer::getBins() const {return snpDataCur->getBins();}
inline int * SnpBuffer::getLcbs() const {return snpDataCur->getLcbs();}
inline int SnpBuffer::getPosEnd() const {return snpDataCur->getPosEnd();}
inline int SnpBuffer::getPosStart() const {return snpDataCur->getPosStart();}
inline int ** SnpBuffer::getSnps() const {return snpDataCur->getSnps();}
inline int * SnpBuffer::getSnpSums() const {return snpDataCur->getSnpSums();}
inline int SnpBuffer::getSnpMax() const {return snpDataCur->getSnpMax();}
inline int SnpBuffer::getSnpSumMax() const {return snpDataCur->getSnpSumMax();}
inline bool SnpBuffer::ready() const {return snpDataCur != 0;}
#endif /* defined(__gavqt__SnpBuffer__) */
