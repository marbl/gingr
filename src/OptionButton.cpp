// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "OptionButton.h"

OptionButton::OptionButton(OptionPanel * newOptions, const QString & label)
: QPushButton(label),
options(newOptions)
{
	setCheckable(true);
	adjustSize();
	connect(options, SIGNAL(closed()), this, SLOT(panelClosed()));
}

void OptionButton::panelClosed()
{
	setChecked(false);
	minimizePanel();
}

void OptionButton::enterEvent(QEvent *)
{
	if ( isChecked() )
	{
		options->setTargetOpacity(.85);
	}
}

void OptionButton::leaveEvent(QEvent *)
{
	if ( isChecked() )
	{
		options->setTargetOpacity(.5);
	}
}

void OptionButton::mouseReleaseEvent(QMouseEvent * event)
{
	QPushButton::mouseReleaseEvent(event);
	
	if ( isChecked() )
	{
		options->restore();
	}
	else
	{
		minimizePanel();
	}
}

void OptionButton::paintEvent(QPaintEvent * event)
{
	QPushButton::paintEvent(event);
	
//	options->move(pos().x(), pos().y() - options->size().height());
}

void OptionButton::update()
{
	QPushButton::update();
//	options->move(pos().x(), pos().y() - options->size().height());
}

void OptionButton::minimizePanel()
{
	QPoint posGlobal = parentWidget()->mapToGlobal(pos());
	options->minimize(posGlobal.x(), posGlobal.y());
}