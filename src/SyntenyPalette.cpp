//
//  SyntenyPalette.cpp
//  gingr
//
//  Created by Brian Ondov on 9/4/13.
//
//

#include "SyntenyPalette.h"

SyntenyPalette::SyntenyPalette()
{
	for ( int i = 0; i < PALETTE_SIZE; i++ )
	{
		int shade = 155 * (float)i / PALETTE_SIZE;
		
		palette[i] = qRgb(50, 50 + shade, 205);
	}
	
//	palette[PALETTE_SIZE - 1] = qRgb(255, 255, 255);
}
