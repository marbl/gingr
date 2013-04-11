//
//  OptionButton.h
//  gavqt
//
//  Created by Brian Ondov on 3/21/13.
//
//

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
