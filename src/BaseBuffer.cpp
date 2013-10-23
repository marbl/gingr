//
//  BaseBuffer.cpp
//  gavqt
//
//  Created by Brian Ondov on 5/3/13.
//
//

#include "BaseBuffer.h"
#include <QPainter>

BaseBuffer::BaseBuffer(int width, int height, bool light, bool snp, bool legend)
{
	charA = new BaseImage(width, height, 'A', light, snp, legend);
	charC = new BaseImage(width, height, 'C', light, snp, legend);
	charG = new BaseImage(width, height, 'G', light, snp, legend);
	charN = new BaseImage(width, height, 'N', light, snp, legend);
	charT = new BaseImage(width, height, 'T', light, snp, legend);
	char_ = new BaseImage(width, height, '-', light, snp, legend);
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

