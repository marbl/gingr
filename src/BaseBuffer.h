// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__BaseBuffer__
#define __gavqt__BaseBuffer__

#include "BaseImage.h"

class BaseBuffer
{
public:
	
	BaseBuffer(int width, int height, bool light, bool snp, bool gaps, bool legend = false);
	~BaseBuffer();
	const QPixmap * image(char chr) const;
	
private:
	
	void paintChar(QPixmap * image, char chr, int size, QColor color, QColor colorFont) const;
	
	QPixmap * charA;
	QPixmap * charC;
	QPixmap * charG;
	QPixmap * charN;
	QPixmap * charT;
	QPixmap * char_;
};

#endif /* defined(__gavqt__BaseBuffer__) */
