// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include <QFrame>

#ifndef gavqt_DrawingArea_h
#define gavqt_DrawingArea_h

class DrawingArea : public QFrame
{
	Q_OBJECT
	
public:
	
	DrawingArea(QWidget *parent = 0);
	~DrawingArea();
	
	const QImage * getBuffer() const;
	int getHeight() const;
	int getWidth() const;
	void setBufferUpdateNeeded();
	
protected:
	
	void clearBuffer() const;
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent * event);
	virtual void updateBuffer();
	
	QImage * imageBuffer;
	int xOffset;
	
private:
	
	bool bufferUpdateNeeded;
};

inline const QImage * DrawingArea::getBuffer() const {return imageBuffer;}
inline int DrawingArea::getHeight() const {return height() - frameWidth() * 2;}
inline int DrawingArea::getWidth() const {return width() - frameWidth() * 2;}
inline void DrawingArea::setBufferUpdateNeeded() {bufferUpdateNeeded = true; update();}

#endif
