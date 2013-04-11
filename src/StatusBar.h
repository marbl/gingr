//
//  StatusBar.h
//  gavqt
//
//  Created by Brian Ondov on 3/26/13.
//
//

#ifndef __gavqt__StatusBar__
#define __gavqt__StatusBar__

#include <QLabel>

class StatusBar : public QLabel
{
	Q_OBJECT
	
public:
	
	StatusBar(QWidget * parent = 0);
	
	void setPosition(int gapped, int ungapped, int offset);
};

#endif /* defined(__gavqt__StatusBar__) */
