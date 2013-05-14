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
	
	BaseBuffer(int width, int height, bool snp);
	~BaseBuffer();
	const QImage * image(char chr) const;
	
private:
	
	void paintChar(QImage * image, char chr, int size, QColor color, QColor colorFont) const;
	
	QImage * charA;
	QImage * charC;
	QImage * charG;
	QImage * charN;
	QImage * charT;
	QImage * char_;
};

#endif /* defined(__gavqt__BaseBuffer__) */
