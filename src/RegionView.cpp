// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "RegionView.h"

void RegionView::draw(QRgb * scanLine, const QRgb * palette, const int * hues, int width, bool rcTrack, int offset, float scale, int partition) const
{
	int gradient;
	int gradientFactor = 1;//(endRef - startRef) / (end - start);
	float startPos;
	float endPos;
	
	//width--;
	
	if ( rcTrack )
	{
		startPos = -end;
		endPos = -start;
	}
	else
	{
		startPos = start;
		endPos = end;
	}
	
	if ( rc != rcRef )
	{
		gradient = endRef * width;
		gradientFactor = -gradientFactor;
	}
	else
	{
		gradient = startRef * width;
	}
	
	if ( gradient == width )
	{
		gradient = width - 1;
	}
	
	for (int i = startPos * width + offset; i <= endPos * width + offset; i++)
	{
		int x = i;
		
		if ( x >= width )
		{
			x -= width;
		}
		
		if ( x < 0 )
		{
			x += width;
		}
		
		if ( scale < 1 )
		{
			int pivot;
			
			if ( x < partition )
			{
				pivot = partition / 2;
			}
			else
			{
				pivot = (partition + width) / 2;
			}
			
			x = pivot + (x - pivot) * scale;
		}
		
		int lightness;
		int lightnessFactor = 50;
		
		if ( scale > 0 )
		{
			lightness = 127 + lightnessFactor - scale * lightnessFactor;
		}
		else
		{
			lightness = 127 - lightnessFactor - scale * lightnessFactor;
		}
		
		scanLine[x] = palette[hues[(int)gradient]];
		
		gradient += gradientFactor;
		
		if ( gradient < 0 )
		{
			gradient = 0;
		}
		if ( gradient == width )
		{
			gradient = width - 1;
		}
	}
}

void RegionView::drawHighlight(QRgb *scanLine, int width, int offset, float hlOffset, bool rcTrack, float scale, int partition) const
{
	int x = int((start + hlOffset) * (rcTrack ? -1 : 1) * width);
	
//	if ( rcTrack )
	{
//		x = width - x;
	}
	
	x += offset;
	
	if ( x < 0 )
	{
		x += width;
	}
	
	if ( x >= width )
	{
		x -= width;
	}
	
	if ( scale < 1 )
	{
		int pivot;
		
		if ( x < partition )
		{
			pivot = partition / 2;
		}
		else
		{
			pivot = (partition + width) / 2;
		}
		
		x = pivot + (x - pivot) * scale;
	}
	
	if ( rcTrack == rc == scale > 0 )
	{
		scanLine[x] = QColor::fromHsl(120, 255, 127).rgb();
		scanLine[x + 1] = QColor::fromHsl(120, 255, 127).rgb();
	}
	else
	{
		scanLine[x] = QColor::fromHsl(300, 255, 127).rgb();
		scanLine[x + 1] = QColor::fromHsl(300, 255, 127).rgb();
	}
}

float wrap(float value, float min, float max)
{
	while ( value < min )
	{
		value += max - min;
	}
	
	while ( value > max )
	{
		value -= max - min;
	}
	
	return value;
}
