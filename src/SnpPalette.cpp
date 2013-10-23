//
//  SnpPalette.cpp
//  gavqt
//
//  Created by Brian Ondov on 6/17/13.
//
//

#include "SnpPalette.h"
#include <QtCore/qmath.h>
#include <QColor>

SnpPalette::SnpPalette(bool light)
{
	for ( int i = 0; i < PALETTE_SIZE; i++ )
	{
		float x = (float)i / PALETTE_SIZE;
		
		int r;
		int g;
		int b;
		
		if ( light )
		{
			r = 256 * x;
			g = 64;
			b = 64;
			//g = x > .5 ? 255 : 255 - 512 * (x - .5);
			//b = x > .5 ? 0 : 512 * x;
			palette[i] = QColor::fromHsl(240 + 90 * x, 245 - 128 * x, 245 - 128 * x).rgb();
		}
		else
		{
			// gnuplot default heatmap colors (rgbformulae 7,5,15)
			//
			
			r = 256 * qSqrt(x);
			g = 256 * qPow(x, 3);
			b = 256 * qSin(2 * 3.1415926 * x);
			
			if ( b < 0 )
			{
				b = 0;
			}
			
			palette[i] = qRgb(r, g, b);
		}
	}
	
	if ( light )
	{
		palette[0] = qRgb(255, 255, 255);
		palette[PALETTE_SIZE - 1] = qRgb(255, 128, 0);
	}
	else
	{
		palette[PALETTE_SIZE - 1] = qRgb(255, 255, 255);
	}
}
