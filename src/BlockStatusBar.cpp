//
//  BlockStatusBar.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/4/13.
//
//

#include "BlockStatusBar.h"
#include <QHBoxLayout>

BlockStatusBar::BlockStatusBar()
: QWidget()
{
	labelPosition = new StatusLabel();
	QHBoxLayout * layout = new QHBoxLayout();
	layout->addWidget(labelPosition);
	layout->setMargin(0);
	layout->setSpacing(3);
	setLayout(layout);
}

void BlockStatusBar::setPosition(int ungapped, int offset)
{
	if ( offset )
	{
		labelPosition->setText(QString("%1 + %2").arg(ungapped).arg(offset));
	}
	else
	{
		labelPosition->setText(QString("%1").arg(ungapped));
	}
}
