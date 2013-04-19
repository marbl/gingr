//
//  StatusLabel.h
//  gavqt
//
//  Created by Brian Ondov on 4/4/13.
//
//

#ifndef __gavqt__StatusLabel__
#define __gavqt__StatusLabel__

#include <QFrame>
#include <QLabel>

class StatusLabel : public QFrame
{
public:
	
	StatusLabel(QWidget * parent = 0);
	
	void setColorBG(QColor color);
	void setColorFG(QColor color);
	void setText(const QString & text);
	void setWidth(int width);
	
private:
	
	QLabel * label;
};

#endif /* defined(__gavqt__StatusLabel__) */
