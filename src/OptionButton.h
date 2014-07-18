// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__OptionButton__
#define __gavqt__OptionButton__

#include <QPushButton>
#include "OptionPanel.h"

class OptionButton : public QPushButton
{
	Q_OBJECT
	
public:
	
	OptionButton(OptionPanel * newOptions, const QString & label);
	
public slots:
	
	void panelClosed();
	void update();
	
protected:
	
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mouseReleaseEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event);
	
private:

	void minimizePanel();
	
	OptionPanel * options;
};

#endif /* defined(__gavqt__OptionButton__) */
