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
#include "SnpPalette.h"

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
	QImage * getRow(int row) const;
	QImage * getSum() const;
	void initialize(const Alignment * newAlignment);
	bool ready() const;
	void update(int posStart, int posEnd, int bins, bool synteny);
	
public slots:
	
	void threadError(QString) {printf("Thread error.\n");}
	void updateFinished();
	
signals:
	
	void updated();
	
private:
	
	void drawSnps(QImage * image, QImage * snps, int top, int bottom, int posStart, int posEnd, int binsTarget, int binsSource) const;
	void swap();
	
	SnpPalette snpPalette;
	const Alignment * alignment;
	SnpData * snpDataNew;
	SnpData * snpDataCur;
	int binsQueue;
	int posStartQueue;
	int posEndQueue;
	bool syntenyQueue;
	int trackCount;
	
	bool wave; // TEMP
	bool updating;
	bool updateNeeded;
};

inline int SnpBuffer::getBins() const {return snpDataCur->getBins();}
inline int * SnpBuffer::getLcbs() const {return snpDataCur->getLcbs();}
inline int SnpBuffer::getPosEnd() const {return snpDataCur->getPosEnd();}
inline int SnpBuffer::getPosStart() const {return snpDataCur->getPosStart();}
inline QImage * SnpBuffer::getRow(int row) const {return snpDataCur->getRow(row);}
inline QImage * SnpBuffer::getSum() const {return snpDataCur->getSum();}
inline bool SnpBuffer::ready() const {return snpDataCur != 0;}
#endif /* defined(__gavqt__SnpBuffer__) */
