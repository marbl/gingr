//
//  SyntenyPalette.h
//  gingr
//
//  Created by Brian Ondov on 9/4/13.
//
//

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
