//
//  BaseBuffer.cpp
//  gavqt
//
//  Created by Brian Ondov on 5/3/13.
//
//

#include "BaseBuffer.h"
#include <QPainter>

BaseBuffer::BaseBuffer(int width, int height, bool snp)
{
	charA = new BaseImage(width, height, 'A', snp);
	charC = new BaseImage(width, height, 'C', snp);
	charG = new BaseImage(width, height, 'G', snp);
	charN = new BaseImage(width, height, 'N', snp);
	charT = new BaseImage(width, height, 'T', snp);
	char_ = new BaseImage(width, height, '-', snp);
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

const QImage * BaseBuffer::image(char chr) const
{
	const QImage * charImage = 0;
	
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

