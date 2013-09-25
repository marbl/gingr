//
//  PhylogenyTreeStatusBar.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/4/13.
//
//

#include "PhylogenyTreeStatusBar.h"
#include <QHBoxLayout>

PhylogenyTreeStatusBar::PhylogenyTreeStatusBar()
: QWidget()
{
	labelName = new StatusLabel();
	labelDistance = new StatusLabel();
	labelBootstrap = new StatusLabel();
	
	QHBoxLayout * layout = new QHBoxLayout();
	
	int labelSize = 60;
	
	labelDistance->setMinimumWidth(labelSize);
	labelDistance->setMaximumWidth(labelSize);
	labelDistance->setToolTip("Branch length");
	labelBootstrap->setMinimumWidth(labelSize);
	labelBootstrap->setMaximumWidth(labelSize);
	labelBootstrap->setToolTip("Bootstrap");
	
	QLabel * labelDistanceIcon = new QLabel(QString("%1").arg(QChar(0xE7, 0x02)));
	QLabel * labelBootstrapIcon = new QLabel("*");
	
	layout->addWidget(labelName, 1);
	layout->addWidget(labelDistanceIcon, 0);
	layout->addWidget(labelDistance, 0);
	layout->addWidget(labelBootstrapIcon, 0);
	layout->addWidget(labelBootstrap, 0);
	
	layout->setMargin(0);
	layout->setSpacing(3);
	
	setLayout(layout);
}

void PhylogenyTreeStatusBar::setName(const QString & name)
{
	labelName->setText(name);
}

void PhylogenyTreeStatusBar::setPhylogenyNode(const PhylogenyNode * phylogenyNode, const QString & name, int leaf)
{
	node = phylogenyNode;
	labelName->setText(name);
	
	QColor color;
	
	if ( leaf == -1 )
	{
		color = qRgb(255, 255, 255);
	}
	else
	{
		color = QColor::fromHsl(leaf * 210 % 360, 50, 235).rgb();
	}
	
	//labelName->setColor(color);
	//labelDistance->setColor(color);
	//labelBootstrap->setColor(color);
	
	if ( node == 0 )
	{
		labelDistance->setText("");
		labelBootstrap->setText("");
		labelBootstrap->setColorFG(qRgb(80, 80, 80));
	}
	else
	{
		labelDistance->setText(QString("%1").arg(node->getDistance()));
		labelBootstrap->setText(QString("%1").arg(node->getBootstrap()));
		
		if ( node->getBootstrap() > .5 || node->getBootstrap() == 0 )
		{
			labelBootstrap->setColorFG(qRgb(80, 80, 80));
		}
		else
		{
			labelBootstrap->setColorFG(qRgb(200, 80, 80));
		}
	}
}
