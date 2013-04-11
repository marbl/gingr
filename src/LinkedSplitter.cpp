//
//  LinkedSplitter.cpp
//  gavqt
//
//  Created by Brian Ondov on 2/4/13.
//
//

#include "LinkedSplitter.h"

void LinkedSplitter::moveSplitter(int pos, int index)
{
	bool oldBlock = blockSignals(true);
	QSplitter::moveSplitter(pos, index);
	blockSignals(oldBlock);
}
