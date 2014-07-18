// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gingr__SyntenyPalette__
#define __gingr__SyntenyPalette__

#include <QRgb>

class SyntenyPalette
{
public:
	
	static const int PALETTE_SIZE = 256;
	
	SyntenyPalette();
	
	QRgb color(int index) const;
	
private:
	
	QRgb palette[PALETTE_SIZE];
};

inline QRgb SyntenyPalette::color(int index) const {return palette[index];}

#endif /* defined(__gingr__SyntenyPalette__) */
