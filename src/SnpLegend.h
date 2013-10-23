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
#include "SyntenyPalette.h"
#include "BaseBuffer.h"

const int baseCount = 6;
const char bases[baseCount] = {'A', 'C', 'G', 'T', 'N', '-'};

class SnpLegend : public DrawingArea
{
public:
	
	SnpLegend();
	~SnpLegend();
	
	void setLightColors(bool light);
	void setShow(bool newShow);
	void setShowBases(bool newShowBases);
	void setShowSynteny(bool newShowSynteny);
	
protected:
	
	void resizeEvent(QResizeEvent * event);
	void updateBuffer();
	
private:
	
	void refreshBaseBuffers();
	
	SnpPalette * palette;
	SnpPalette * paletteLight;
	SyntenyPalette paletteSynteny;
	BaseBuffer * baseBufferRef;
	BaseBuffer * baseBufferSnp;
	int baseSize;
	bool show;
	bool showBases;
	bool showSynteny;
	bool lightColors;
};


#endif /* defined(__gavqt__SnpLegend__) */
