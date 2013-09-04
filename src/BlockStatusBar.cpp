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
	snpLegend = new SnpLegend();
	labelPosition = new StatusLabel();
	labelOffset = new StatusLabel();
	labelName = new StatusLabel();
	
	snpLegend->setMinimumWidth(12 * 18 + 2);
	snpLegend->setMaximumWidth(12 * 18 + 2);
	labelPosition->setWidth(80);
	labelOffset->setWidth(60);
	
	QLabel * textOffset = new QLabel("+");
	
	QHBoxLayout * layout = new QHBoxLayout();
	
	layout->addWidget(snpLegend, 0);
	layout->addWidget(labelName, 1);
	layout->addWidget(labelPosition, 0);
	layout->addWidget(textOffset, 0);
	layout->addWidget(labelOffset, 0);
	
	layout->setMargin(0);
	layout->setSpacing(3);
	
	setLayout(layout);
}

void BlockStatusBar::setLegendBases(bool bases)
{
	snpLegend->setShowBases(bases);
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

void BlockStatusBar::setSynteny(bool synteny)
{
	snpLegend->setShowSynteny(synteny);
}
