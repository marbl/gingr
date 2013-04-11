//
//  LinkedSplitter.h
//  gavqt
//
//  Created by Brian Ondov on 2/4/13.
//
//

#ifndef __gavqt__LinkedSplitter__
#define __gavqt__LinkedSplitter__

#include <QSplitter>

class LinkedSplitter : public QSplitter
{
	Q_OBJECT
	
public slots:
	
	void moveSplitter(int pos, int index);
};

#endif /* defined(__gavqt__LinkedSplitter__) */
