//
//  StatusLabel.h
//  gavqt
//
//  Created by Brian Ondov on 4/4/13.
//
//

#ifndef __gavqt__StatusLabel__
#define __gavqt__StatusLabel__

#include "DrawingArea.h"
#include <QLabel>

class StatusLabel : public DrawingArea
{
public:
	
	StatusLabel(QWidget * parent = 0);
	
	void setColorBG(QColor color);
	void setColorFG(QColor color);
	void setText(const QString & text);
	
private:
	
	QLabel * label;
};

#endif /* defined(__gavqt__StatusLabel__) */
