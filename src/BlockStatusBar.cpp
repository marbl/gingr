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
	labelIns = new StatusLabel();
	labelDel = new StatusLabel();
	
	snpLegend->setMinimumWidth(12 * 18 + 2);
	snpLegend->setMaximumWidth(12 * 18 + 2);
	labelIns->setMinimumWidth(25);
	labelDel->setMinimumWidth(25);
	labelIns->setText(tr("Ins"));
	labelDel->setText(tr("Del"));
	labelIns->setToolTip(tr("Highlight gaps for insertions"));
	labelDel->setToolTip(tr("Highlight gaps for deletions"));
	labelPosition->setWidth(80);
	labelOffset->setWidth(60);
	labelPosition->setToolTip(tr("Reference position"));
	labelOffset->setToolTip(tr("Insertion position"));
	
	QLabel * textOffset = new QLabel("+");
	
	QHBoxLayout * layout = new QHBoxLayout();
	
	layout->addWidget(snpLegend, 0);
	layout->addWidget(labelIns, 0);
	layout->addWidget(labelDel, 0);
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

void BlockStatusBar::setShowGaps(bool gaps)
{
	snpLegend->setShowGaps(gaps);
	updateColors();
}

void BlockStatusBar::setShowIns(bool ins)
{
	snpLegend->setShowIns(ins);
	updateColors();
}

void BlockStatusBar::setShowDel(bool del)
{
	snpLegend->setShowDel(del);
	updateColors();
}

void BlockStatusBar::setSynteny(bool synteny)
{
	snpLegend->setShowSynteny(synteny);
}

void BlockStatusBar::updateColors()
{
	if ( snpLegend->getShowGaps() )
	{
		if ( snpLegend->getShowIns() )
		{
			labelIns->setColorBG(colorGapShowBG);
			labelIns->setColorFG(colorGapShowFG);
		}
		else
		{
			labelIns->setColorBG(colorGapBG);
			labelIns->setColorFG(colorGapFG);
		}
		
		if ( snpLegend->getShowDel() )
		{
			labelDel->setColorBG(colorGapShowBG);
			labelDel->setColorFG(colorGapShowFG);
		}
		else
		{
			labelDel->setColorBG(colorGapBG);
			labelDel->setColorFG(colorGapFG);
		}
	}
	else
	{
		if ( snpLegend->getShowIns() )
		{
			labelIns->setColorBG(colorGapShowDisabledBG);
			labelIns->setColorFG(colorGapShowDisabledFG);
		}
		else
		{
			labelIns->setColorBG(colorGapDisabledBG);
			labelIns->setColorFG(colorGapDisabledFG);
		}
		
		if ( snpLegend->getShowDel() )
		{
			labelDel->setColorBG(colorGapShowDisabledBG);
			labelDel->setColorFG(colorGapShowDisabledFG);
		}
		else
		{
			labelDel->setColorBG(colorGapDisabledBG);
			labelDel->setColorFG(colorGapDisabledFG);
		}
	}
	
	labelIns->setText(tr("Ins"));
	labelDel->setText(tr("Del"));
}
