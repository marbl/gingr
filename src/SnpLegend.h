// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

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
	
	bool getShowGaps() const;
	bool getShowIns() const;
	bool getShowDel() const;
	void setLightColors(bool light);
	void setShow(bool newShow);
	void setShowBases(bool newShowBases);
	void setShowGaps(bool newShowGaps);
	void setShowIns(bool newShowIns);
	void setShowDel(bool newShowDel);
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
	bool showGaps;
	bool showIns;
	bool showDel;
	bool showBases;
	bool showSynteny;
	bool lightColors;
};

inline bool SnpLegend::getShowGaps() const {return showGaps;}
inline bool SnpLegend::getShowIns() const {return showIns;}
inline bool SnpLegend::getShowDel() const {return showDel;}

#endif /* defined(__gavqt__SnpLegend__) */
