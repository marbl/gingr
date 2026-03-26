// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__SnpPalette__
#define __gavqt__SnpPalette__

#include <QRgb>

class SnpPalette
{
public:
		
	enum ColorMode {
		NORMAL,
		SYN_NONSYN
	};

	static const int PALETTE_SIZE = 1 << 10;

	SnpPalette(bool light, ColorMode mode = NORMAL);

	QRgb color(int index) const;

private:
	
	QRgb palette[PALETTE_SIZE];
	ColorMode colorMode;
};



inline QRgb SnpPalette::color(int index) const {return palette[index];}

#endif /* defined(__gavqt__SnpPalette__) */
