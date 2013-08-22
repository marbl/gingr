//
//  SnpLegend.h
//  gavqt
//
//  Created by Brian Ondov on 6/17/13.
//
//

#ifndef __gavqt__SnpLegend__
#define __gavqt__SnpLegend__

#include <DrawingArea.h>
#include "SnpPalette.h"
#include "BaseBuffer.h"

const int baseCount = 6;
const char bases[baseCount] = {'A', 'C', 'G', 'T', 'N', '-'};

class SnpLegend : public DrawingArea
{
public:
	
	SnpLegend();
	~SnpLegend();
	
	void setShow(bool newShow);
	void setShowBases(bool newShowBases);
	
protected:
	
	void resizeEvent(QResizeEvent * event);
	void updateBuffer();
	
private:
	
	SnpPalette palette;
	BaseBuffer * baseBufferRef;
	BaseBuffer * baseBufferSnp;
	int baseSize;
	bool show;
	bool showBases;
};

#endif /* defined(__gavqt__SnpLegend__) */
