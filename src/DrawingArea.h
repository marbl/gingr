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
	~DrawingArea();
	
	int getHeight() const;
	int getWidth() const;
	
protected:
	
	void clearBuffer();
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent * event);
	void setBufferUpdateNeeded();
	virtual void updateBuffer();
	
	QImage * imageBuffer;
	int xOffset;
	
private:
	
	bool bufferUpdateNeeded;
};

inline int DrawingArea::getHeight() const {return height() - frameWidth() * 2;}
inline int DrawingArea::getWidth() const {return width() - frameWidth() * 2;}
inline void DrawingArea::setBufferUpdateNeeded() {bufferUpdateNeeded = true; update();}

#endif
