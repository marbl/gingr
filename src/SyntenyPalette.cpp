// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

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
