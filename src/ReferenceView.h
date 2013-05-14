//
//  ReferenceView.h
//  gavqt
//
//  Created by Brian Ondov on 4/9/13.
//
//

#ifndef __gavqt__ReferenceView__
#define __gavqt__ReferenceView__

#include "DrawingArea.h"
#include "SnpBuffer.h"
#include "Alignment.h"

class ReferenceView : public DrawingArea
{
	Q_OBJECT
	
public:
	
	ReferenceView();
	
	void setAlignment(const Alignment * newAlignment);
	void setSnpBuffer(const SnpBuffer * newSnpBuffer);
	void setWindow(int newPosStart, int newPosEnd);
	
public slots:
	
	void updateSnpsFinished();
	
protected:
	
	void updateBuffer();
	
private:
	
	const Alignment * alignment;
	const SnpBuffer * snpBuffer;
	
	int posStart;
	int posEnd;
};

#endif /* defined(__gavqt__ReferenceView__) */
