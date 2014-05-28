//
//  BlockViewMain.h
//  gavqt
//
//  Created by Brian Ondov on 3/29/13.
//
//

#ifndef __gavqt__BlockViewMain__
#define __gavqt__BlockViewMain__

#include "BlockView.h"

#include "BaseBuffer.h"

class BlockViewMain : public BlockView
{
	Q_OBJECT
	
public:
	
	BlockViewMain();
	~BlockViewMain();
	
	void update();
	
public slots:
	
	void setLcbHover(int lcb, float offset);
	void setReference();
	void setWindow(int start, int end);
	void updateSnpsFinished();
	
protected:
	
	void leaveEvent(QEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent * event);
	void updateBuffer();
	void updateSnps();
	void updateTrackCursor();
	void wheelEvent(QWheelEvent * event);
	
signals:
	
	void signalContextMenu(bool inContextMenu);
	void signalMouseWheel(int delta);
	void signalTrackReference(int trackReferenceNew);
	void signalToggleSynteny();
	
private:
	
	int computeTrackHeight(int track) const;
	void drawLines() const;
	void drawSequence() const;
	void drawSequenceRef(QImage * image, const BaseBuffer * baseBufferRef, const BaseBuffer * baseBufferSnp, const BaseImage * gapImage, int firstSnp) const;
	void panTo(int position);
	void updateMousePosition();
	
	char * seq;
	int seqCount;
	bool mouseDown;
	int mouseDownX;
	int mouseDownY;
	int mousePosition;
	int mouseVelocity;
	int mouseXLast;
	int offset;
	int lcbHover;
	float lcbHoverOffset;
	unsigned int mouseDownPosition;
	bool clicking;
};

#endif /* defined(__gavqt__BlockViewMain__) */
