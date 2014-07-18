// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "BaseBuffer.h"
#include <QPainter>

BaseBuffer::BaseBuffer(int width, int height, bool light, bool snp, bool gaps, bool legend)
{
	charA = new BaseImage(width, height, 'A', light, snp, gaps, legend);
	charC = new BaseImage(width, height, 'C', light, snp, gaps, legend);
	charG = new BaseImage(width, height, 'G', light, snp, gaps, legend);
	charN = new BaseImage(width, height, 'N', light, snp, gaps, legend);
	charT = new BaseImage(width, height, 'T', light, snp, gaps, legend);
	char_ = new BaseImage(width, height, '-', light, snp, gaps, legend);
}

BaseBuffer::~BaseBuffer()
{
	delete charA;
	delete charC;
	delete charG;
	delete charN;
	delete charT;
	delete char_;
}

const QPixmap * BaseBuffer::image(char chr) const
{
	const QPixmap * charImage = 0;
	
	switch (chr)
	{
		case 'A': charImage = charA; break;
		case 'C': charImage = charC; break;
		case 'G': charImage = charG; break;
		case 'N': charImage = charN; break;
		case 'T': charImage = charT; break;
		case '-': charImage = char_; break;
	}
	
	return charImage;
}

