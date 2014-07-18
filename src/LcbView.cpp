// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "LcbView.h"

void LcbView::setAlignment(const Alignment * newAlignment)
{
	alignment = newAlignment;
	setToolTip("Phylogenetic discordance by LCB");
}

void LcbView::setWindow(int newStart, int newEnd)
{
	start = newStart;
	end = newEnd;
	setBufferUpdateNeeded();
}

void LcbView::updateBuffer()
{
	clearBuffer();
	
	int bins = getWidth();
	
	if ( bins <= 0 )
	{
		return;
	}
	
	int windowSize = end - start + 1;
	float binWidth = (float)bins / windowSize;
	float con[bins];
	float conMax = 0;
	
	memset(con, 0, sizeof(float) * bins);
	
	for ( int i = alignment->getNextLcb(start); i < alignment->getLcbCount() && alignment->getLcb(i).startGapped <= end; i++ )
	{
		const Alignment::Lcb & lcb = alignment->getLcb(i);
		int posStartLcb = lcb.startGapped;
		int posEndLcb = posStartLcb + lcb.lengthGapped - 1;
		int binStart = posStartLcb < start ? 0 : (posStartLcb - start) * binWidth;
		int binEnd = posEndLcb > end ? bins - 1 : (posEndLcb - start) * binWidth;
		
		for ( int j = binStart; j <= binEnd; j++ )
		{
			if ( lcb.concordance > con[j] )
			{
				con[j] = lcb.concordance;
			}
			
			if ( lcb.concordance > conMax )
			{
				conMax = lcb.concordance;
			}
		}
	}
	
	for ( int i = 0; i < bins; i++ )
	{
		for ( int j = 0; j < con[i] * getHeight() && j < getHeight(); j++ )
			;//((QRgb *)imageBuffer->scanLine(getHeight() - j - 1))[i] = qRgb(255, 0, 0);
	}
}
