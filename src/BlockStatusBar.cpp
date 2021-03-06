// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "BlockStatusBar.h"
#include <QHBoxLayout>

BlockStatusBar::BlockStatusBar()
: QWidget()
{
	snpLegend = new SnpLegend();
	labelPosition = new StatusLabel();
	labelOffset = new StatusLabel();
	labelTracks = new StatusLabel();
	labelName = new StatusLabel();
	labelGaps = new StatusLabel();
	labelIns = new StatusLabel();
	labelDel = new StatusLabel();
	
	snpLegend->setMinimumWidth(12 * 16 + 2);
	snpLegend->setMaximumWidth(12 * 16 + 2);
	labelGaps->setMinimumWidth(30);
	labelIns->setMinimumWidth(30);
	labelDel->setMinimumWidth(30);
	labelGaps->setAlignment(Qt::AlignHCenter);
	labelIns->setAlignment(Qt::AlignHCenter);
	labelDel->setAlignment(Qt::AlignHCenter);
	labelGaps->setText(tr("Gap"));
	labelIns->setText(tr("Ins"));
	labelDel->setText(tr("Del"));
	labelGaps->setToolTip(tr("Highlight gaps"));
	labelIns->setToolTip(tr("Highlight gaps for insertions"));
	labelDel->setToolTip(tr("Highlight gaps for deletions"));
	labelPosition->setWidth(80);
	labelOffset->setWidth(60);
	labelPosition->setToolTip(tr("Reference position"));
	labelOffset->setToolTip(tr("Insertion position"));
	
	labelTracks->setMinimumWidth(50);
	
	labelName->setMinimumWidth(30);
	labelName->setToolTip(tr("Core alignment"));
	
	QLabel * textOffset = new QLabel("+");
	
	QHBoxLayout * layout = new QHBoxLayout();
	
	//layout->addWidget(labelGaps, 0);
	//layout->addWidget(labelIns, 0);
	//layout->addWidget(labelDel, 0);
	layout->addWidget(new QLabel(tr("Tracks:")));
	layout->addWidget(labelTracks, 0);
	layout->addWidget(new QLabel(tr("Core:")));
	layout->addWidget(labelName, 1);
	layout->addWidget(labelPosition, 0);
	layout->addWidget(textOffset, 0);
	layout->addWidget(labelOffset, 0);
	layout->addWidget(snpLegend, 0);
	
	layout->setMargin(0);
	layout->setSpacing(3);
	
	setLayout(layout);
}

void BlockStatusBar::clear()
{
	labelTracks->setText("");
	labelName->setText(tr(""));
	labelName->setColorBG(qRgb(245, 245, 245));
}

void BlockStatusBar::setCore(float core)
{
	labelName->setText(QString("%1\%").arg(int(core * 100)));
	
	QColor color;
	
	if ( core < .3 )
	{
		labelName->setColorBG(colorCoreBad);
	}
	else if ( core < .5 )
	{
		labelName->setColorBG(colorCoreOk);
	}
	else
	{
		labelName->setColorBG(colorCoreGood);
	}
	
	labelName->setColorFG(qRgb(64, 64, 64));
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

void BlockStatusBar::setTracks(int tracks)
{
	labelTracks->setText(QString("%1").arg(tracks));
}

void BlockStatusBar::updateColors()
{
	if ( snpLegend->getShowGaps() )
	{
		labelGaps->setColorBG(colorGapShowBG);
		labelGaps->setColorFG(colorGapShowFG);
		
		labelIns->setDisabled(false);
		labelDel->setDisabled(false);
		
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
		labelGaps->setColorBG(colorGapBG);
		labelGaps->setColorFG(colorGapFG);
		
		labelIns->setDisabled(true);
		labelDel->setDisabled(true);
		
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
