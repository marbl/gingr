//
//  BlockStatusBar.h
//  gavqt
//
//  Created by Brian Ondov on 4/4/13.
//
//

#ifndef __gavqt__BlockStatusBar__
#define __gavqt__BlockStatusBar__

#include <QWidget>
#include "StatusLabel.h"

class BlockStatusBar : public QWidget
{
public:
	
	BlockStatusBar();
	
	void setPosition(int ungapped, int offset);
	
private:
	
	StatusLabel * labelPosition;
};

#endif /* defined(__gavqt__BlockStatusBar__) */
