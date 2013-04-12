//
//  BlockView.h
//  gavqt
//
//  Created by Brian Ondov on 11/6/12.
//
//

#ifndef __gavqt__BlockView__
#define __gavqt__BlockView__

#include "BlockView.h"
#include "Alignment.h"
#include "TrackListView.h"
#include "Tween.h"
#include "Timer.h"
#include "SnpBuffer.h"
#include <QDateTime>
#include <QImage>

const int PALETTE_SIZE = 1 << 10;
const unsigned int SNP_WINDOW = 1 << 15;

class BlockView : public TrackListView
{
	Q_OBJECT
	
public:
	
	BlockView();
	~BlockView();
	
	void setAlignment(const Alignment * newAlignment);
	void update();

public slots:
	
	void setLcbHover(int lcb, float offset);
	void updateSnpsFinished();
	void updateSnpsNeeded();
	
protected:
	
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent * event);
	void updateBuffer();
	virtual void updateSnps();
	void wheelEvent(QWheelEvent * event);
	
	const Alignment * alignment;
	int refSize;
	int posStart;
	int posEnd;
	
signals:
	
	void positionChanged(int ungapped, int gapped, int offset);
	void signalLcbHoverChange(int lcb, float offset);
	void windowChanged(int start, int end);
	
private:
	
	void drawSnps() const;
	void panTo(int position);
	
	RegionVector refByLcb;
	int lcbHover;
	float lcbHoverOffset;
	float zoom;
	unsigned int ** snpsMax;
	SnpBuffer snpsCenter;
	SnpBuffer snpsLeft;
	SnpBuffer snpsRight;
	int snpMaxTarget;
	int snpMaxCur;
	Tween snpMax;
	Timer snpMaxTimer;
	bool mouseDown;
	int mouseDownX;
	int mouseDownY;
	unsigned int mouseDownPosition;
	int mouseVelocity;
	int mouseXLast;
	bool wave;
	int offset;
	QRgb snpPalette[1024];
	QDateTime timeZoomIn;
	QDateTime timeZoomOut;
	bool clicking;
};

#endif /* defined(__gavqt__BlockView__) */
