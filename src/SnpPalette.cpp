//
//  SnpPalette.cpp
//  gavqt
//
//  Created by Brian Ondov on 6/17/13.
//
//

#include "SnpPalette.h"
#include <QtCore/qmath.h>

SnpPalette::SnpPalette()
{
	for ( int i = 0; i < PALETTE_SIZE; i++ )
	{
		float x = (float)i / PALETTE_SIZE;
		
		// gnuplot default heatmap colors (rgbformulae 7,5,15)
		//
		int r = 256 * qSqrt(x);
		int g = 256 * qPow(x, 3);
		int b = 256 * qSin(2 * 3.1415926 * x);
		
		if ( b < 0 )
		{
			b = 0;
		}
		
		palette[i] = qRgb(r, g, b);
	}
	
	palette[PALETTE_SIZE - 1] = qRgb(255, 255, 255);
}
