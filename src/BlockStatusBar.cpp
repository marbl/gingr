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
	labelOffset = new StatusLabel();
	labelName = new StatusLabel();
	
	labelPosition->setWidth(80);
	labelOffset->setWidth(60);
	
	QLabel * textOffset = new QLabel("+");
	
	QHBoxLayout * layout = new QHBoxLayout();
	
	layout->addWidget(labelName, 1);
	layout->addWidget(labelPosition, 0);
	layout->addWidget(textOffset, 0);
	layout->addWidget(labelOffset, 0);
	
	layout->setMargin(0);
	layout->setSpacing(3);
	
	setLayout(layout);
}

void BlockStatusBar::setPosition(int ungapped, int offset)
{
	if ( ungapped == -1 )
	{
		labelPosition->setText("");
		labelOffset->setText("");
	}
	else
	{
		labelPosition->setText(QString("%1").arg(ungapped + 1));
		
		if ( offset )
		{
			labelOffset->setText(QString("%1").arg(offset));
		}
		else
		{
			labelOffset->setText("");
		}
	}
}
