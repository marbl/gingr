//
//  BaseBuffer.cpp
//  gavqt
//
//  Created by Brian Ondov on 5/3/13.
//
//

#include "BaseBuffer.h"
#include <QPainter>

BaseBuffer::BaseBuffer(int width, int height, bool snp, bool legend)
{
	charA = new BaseImage(width, height, 'A', snp, legend);
	charC = new BaseImage(width, height, 'C', snp, legend);
	charG = new BaseImage(width, height, 'G', snp, legend);
	charN = new BaseImage(width, height, 'N', snp, legend);
	charT = new BaseImage(width, height, 'T', snp, legend);
	char_ = new BaseImage(width, height, '-', snp, legend);
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

