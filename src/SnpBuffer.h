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
#include "SyntenyPalette.h"

class SnpBuffer : public QObject
{
	Q_OBJECT

public:
	
	SnpBuffer();
	~SnpBuffer();
	
	void drawSnpSums(QImage * image, int top, int bottom, int posStart, int posEnd, int bins) const;
	void drawSnps(QImage * image, int row, int top, int bottom, int posStart, int posEnd, int bins) const;
	int getBins() const;
	int * getLcbs() const;
	bool getLightColors() const;
	int getMax() const;
	int getMaxView() const;
	int getPosEnd() const;
	int getPosStart() const;
	QImage * getRow(int row) const;
	int getShowGaps() const;
	QImage * getSum() const;
	bool getSynteny() const;
	int getTrackMin() const;
	int getTrackMax() const;
	void initialize(const Alignment * newAlignment, const std::vector<int> * idByTrackNew);
	bool ready() const;
	void update(int posStart, int posEnd, int bins, int trackMin, int trackMax, bool synteny, bool light, int gaps);
	
public slots:
	
	void threadError(QString) {printf("Thread error.\n");}
	void updateFinished();
	
signals:
	
	void updated();
	
private:
	
	void drawSnps(QImage * image, QImage * snps, int top, int bottom, int posStart, int posEnd, int binsTarget, int binsSource) const;
	void swap();
	
	SnpPalette * snpPaletteDark;
	SnpPalette * snpPaletteLight;
	SyntenyPalette syntenyPalette;
	const Alignment * alignment;
	SnpData * snpDataNew;
	SnpData * snpDataCur;
	const std::vector<int> * idByTrack;
	int binsQueue;
	bool lightQueue;
	int posStartQueue;
	int posEndQueue;
	int trackMinQueue;
	int trackMaxQueue;
	bool syntenyQueue;
	int gapsQueue;
	int trackCount;
	
	bool wave; // TEMP
	bool updating;
	bool updateNeeded;
};

inline int SnpBuffer::getBins() const {return snpDataCur->getBins();}
inline int * SnpBuffer::getLcbs() const {return snpDataCur->getLcbs();}
inline bool SnpBuffer::getLightColors() const {return snpDataCur->getLightColors();}
inline int SnpBuffer::getPosEnd() const {return snpDataCur->getPosEnd();}
inline int SnpBuffer::getMax() const {return snpDataCur->getSnpMax();}
inline int SnpBuffer::getMaxView() const {return snpDataCur->getSnpMaxView();}
inline int SnpBuffer::getPosStart() const {return snpDataCur->getPosStart();}
inline QImage * SnpBuffer::getRow(int row) const {return snpDataCur->getRow(row);}
inline int SnpBuffer::getShowGaps() const {return snpDataCur->getShowGaps();}
inline QImage * SnpBuffer::getSum() const {return snpDataCur->getSum();}
inline bool SnpBuffer::getSynteny() const {return snpDataCur->getSynteny();}
inline int SnpBuffer::getTrackMin() const {return snpDataCur->getTrackMin();}
inline int SnpBuffer::getTrackMax() const {return snpDataCur->getTrackMax();}

inline bool SnpBuffer::ready() const {return snpDataCur != 0;}
#endif /* defined(__gavqt__SnpBuffer__) */
