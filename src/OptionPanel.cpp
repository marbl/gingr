//
//  OptionPanel.cpp
//  gavqt
//
//  Created by Brian Ondov on 3/21/13.
//
//

#include "OptionPanel.h"
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QMoveEvent>

OptionPanel::OptionPanel(QWidget * parent)
: QWidget(parent)
{
	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	setAutoFillBackground(true);
	opEffect = new QGraphicsOpacityEffect(this);
//	opEffect->setOpacity(.5);
	tweenOpacity.initialize(0, 0);
//	setGraphicsEffect(opEffect);
//	setFrameStyle(Box);
//	setVisible(false);
	minimized = true;
	restoring = false;
}

void OptionPanel::enter()
{
	timer.initialize(180);
	setTargetOpacity(.85);
}

void OptionPanel::leave()
{
	timer.initialize(180);
	setTargetOpacity(.5);
}

void OptionPanel::minimize(int targetX, int targetY)
{
	tweenOpacity.setTarget(0);
	tweenX.setTarget(targetX);
	tweenY.setTarget(targetY);
	timer.initialize(500);
	minimized = true;
	update();
}

void OptionPanel::restore()
{
	minimized = false;
	restoring = true;
	tweenX.setTarget(x);
	tweenY.setTarget(y);
	setTargetOpacity(.85);
	timer.initialize(150);
}

void OptionPanel::setTargetOpacity(float target)
{
	tweenOpacity.setTarget(target);
	
	if ( tweenOpacity.getValue() == 0 )
	{
		setWindowOpacity(.01); // HACK to get paint event
		setVisible(true);
	}
	
	update();
}

void OptionPanel::closeEvent(QCloseEvent * event)
{
	event->ignore();
	emit closed();
}

void OptionPanel::enterEvent(QEvent *)
{
	if ( ! minimized && ! restoring )
	{
		enter();
	}
}

void OptionPanel::leaveEvent(QEvent *)
{
	if ( ! minimized && ! restoring )
	{
		leave();
	}
}

void OptionPanel::moveEvent(QMoveEvent * event)
{
	QWidget::moveEvent(event);
	
	if ( ! minimized && ! restoring )
	{
		x = event->pos().x();
		y = event->pos().y();
		tweenX.initialize(x, x);
		tweenY.initialize(y, y);
	}
}

void OptionPanel::paintEvent(QPaintEvent * event)
{
	timer.update();
	tweenOpacity.update(timer.getProgress());
	tweenX.update(timer.getProgress());
	tweenY.update(timer.getProgress());
	
	move(tweenX.getValue(), tweenY.getValue());
	
	if ( timer.getProgress() == 1 )
	{
		if ( restoring )
		{
			tweenX.initialize(x, x);
			tweenY.initialize(y, y);
		}
		
		restoring = false;
	}
	else
	{
		update();
	}
	
//	opEffect->setOpacity(tweenOpacity.getValue());
	if ( windowOpacity() != tweenOpacity.getValue() )
	{
		setWindowOpacity(tweenOpacity.getValue());
	}
	
	if ( tweenOpacity.getValue() == 0 )
	{
		setVisible(false);
	}
	
	QWidget::paintEvent(event);
}

