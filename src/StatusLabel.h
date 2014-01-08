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
	Q_OBJECT
	
public:
	
	StatusLabel(QWidget * parent = 0);
	
	void setAlignment(Qt::Alignment alignment);
	void setColorBG(QColor color);
	void setColorFG(QColor color);
	void setText(const QString & text);
	void setWidth(int width);
	
signals:
	
	void clicked();
	
protected:
	
	void mousePressEvent(QMouseEvent *);
	
private:
	
	QLabel * label;
};

#endif /* defined(__gavqt__StatusLabel__) */
