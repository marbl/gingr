// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "SnpPalette.h"
#include <QtCore/qmath.h>
#include <QColor>

SnpPalette::SnpPalette(bool light, ColorMode mode)
{
	colorMode = mode;
	
	if (mode == SYN_NONSYN)
	{
		// Synonymous/Non-synonymous color scheme
		for (int i = 0; i < PALETTE_SIZE; i++)
		{
			float x = (float)i / PALETTE_SIZE;
			
			if (light)
			{
				// Light mode: Blue for synonymous, Red for non-synonymous
				// Lower half = blue (synonymous), upper half = red (non-synonymous)
				if (x < 0.5)
				{
					// Synonymous: white -> light blue -> blue
					int blend = (int)(x * 2 * 255);
					palette[i] = qRgb(255 - blend/2, 255 - blend/2, 255);
				}
				else
				{
					// Non-synonymous: white -> orange -> red
					int blend = (int)((x - 0.5) * 2 * 255);
					palette[i] = qRgb(255, 255 - blend, 128 - blend/2);
				}
			}
			else
			{
				// Dark mode: Blue for synonymous, Red for non-synonymous
				if (x < 0.5)
				{
					// Synonymous: dark -> cyan/blue
					int blend = (int)(x * 2 * 255);
					palette[i] = qRgb(0, blend/2, blend);
				}
				else
				{
					// Non-synonymous: dark -> orange/red
					int blend = (int)((x - 0.5) * 2 * 255);
					palette[i] = qRgb(blend, blend/2, 0);
				}
			}
		}
		
		if (light)
		{
			palette[0] = qRgb(255, 255, 255);
			palette[PALETTE_SIZE - 1] = qRgb(255, 0, 0); // Bright red for high non-syn
		}
		else
		{
			palette[0] = qRgb(0, 0, 0);
			palette[PALETTE_SIZE - 1] = qRgb(255, 128, 0); // Orange for high non-syn
		}
	}
	else
	{
		// Original NORMAL color scheme
		for (int i = 0; i < PALETTE_SIZE; i++)
		{
			float x = (float)i / PALETTE_SIZE;
			
			int r;
			int g;
			int b;
			
			if (light)
			{
				r = 256 * x;
				g = 64;
				b = 64;
				palette[i] = QColor::fromHsl(240 + 120 * x, 250 - 128 * x, 250 - 128 * x).rgb();
			}
			else
			{
				r = 256 * qSqrt(x);
				g = 256 * qPow(x, 3);
				b = 256 * qSin(2 * 3.1415926 * x);
				
				if (b < 0)
				{
					b = 0;
				}
				
				palette[i] = qRgb(r, g, b);
			}
		}
		
		if (light)
		{
			palette[0] = qRgb(255, 255, 255);
			palette[PALETTE_SIZE - 1] = qRgb(255, 128, 0);
		}
		else
		{
			palette[PALETTE_SIZE - 1] = qRgb(255, 255, 255);
		}
	}
}