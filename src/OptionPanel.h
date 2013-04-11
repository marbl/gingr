//
//  OptionPanel.h
//  gavqt
//
//  Created by Brian Ondov on 3/21/13.
//
//

#ifndef __gavqt__OptionPanel__
#define __gavqt__OptionPanel__

#include <QFrame>
#include <QGraphicsOpacityEffect>
#include "Tween.h"
#include "Timer.h"

class OptionPanel : public QWidget
{
	Q_OBJECT
	
public:
	
	OptionPanel(QWidget * newOptions);
	
	void enter();
	void leave();
	void minimize(int targetX, int targetY);
	void restore();
	void setTargetOpacity(float target);
	
protected:
	
	void closeEvent(QCloseEvent * event);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void moveEvent(QMoveEvent * event);
	void paintEvent(QPaintEvent * event);
	
signals:
	
	void closed();
	
private:
	
	QGraphicsOpacityEffect * opEffect;
	Tween tweenOpacity;
	Timer timer;
	Tween tweenX;
	Tween tweenY;
	int x;
	int y;
	bool minimized;
	bool restoring;
};

#endif /* defined(__gavqt__OptionPanel__) */
