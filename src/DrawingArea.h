//
//  DrawingArea.h
//  gavqt
//
//  Created by Brian Ondov on 8/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <QFrame>

#ifndef gavqt_DrawingArea_h
#define gavqt_DrawingArea_h

class DrawingArea : public QFrame
{
	Q_OBJECT
	
public:
	
	DrawingArea(QWidget *parent = 0);
	
	public slots:
	
protected:
	
	void paintEvent(QPaintEvent *event);
	
private:
	
};

#endif
