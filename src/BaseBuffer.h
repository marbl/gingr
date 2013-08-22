//
//  BaseBuffer.h
//  gavqt
//
//  Created by Brian Ondov on 5/3/13.
//
//

#ifndef __gavqt__BaseBuffer__
#define __gavqt__BaseBuffer__

#include "BaseImage.h"

class BaseBuffer
{
public:
	
	BaseBuffer(int width, int height, bool snp, bool legend = false);
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
