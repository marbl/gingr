// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "BaseBuffer.h"
#include <QPainter>

BaseBuffer::BaseBuffer(int width, int height, bool light, bool snp, bool gaps, bool legend, bool synonymous)
{
	charA = new BaseImage(width, height, 'A', light, snp, gaps, legend, synonymous);
	charC = new BaseImage(width, height, 'C', light, snp, gaps, legend, synonymous);
	charG = new BaseImage(width, height, 'G', light, snp, gaps, legend, synonymous);
	charN = new BaseImage(width, height, 'N', light, snp, gaps, legend, synonymous);
	charT = new BaseImage(width, height, 'T', light, snp, gaps, legend, synonymous);
	char_ = new BaseImage(width, height, '-', light, snp, gaps, legend, synonymous);
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

