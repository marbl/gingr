//
//  FilterControl.cpp
//  gavqt
//
//  Created by Brian Ondov on 2/19/13.
//
//

#include "FilterControl.h"
#include <QCheckBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QPushButton>

FilterControl::FilterControl(QWidget * parent)
: OptionPanel(parent)
{
	alignment = 0;
}

void FilterControl::setAlignment(Alignment *newAlignment)
{
	if ( alignment )
	{
		qDeleteAll(children());
	}
	
	alignment = newAlignment;
	QVBoxLayout * layout = new QVBoxLayout();
	
	QButtonGroup * buttonGroupShow = new QButtonGroup(this);
	QCheckBox * checkBoxPass = new QCheckBox("\nPASS\n");
	QRadioButton * buttonHide = new QRadioButton("hide");
	QRadioButton * buttonShow = new QRadioButton("show");
	QPushButton * buttonScale = new QPushButton("Scale");
	buttonGroupShow->addButton(buttonShow);
	buttonGroupShow->addButton(buttonHide);
	buttonGroupShow->setId(buttonHide, 0);
	buttonGroupShow->setId(buttonShow, 1);
	//layout->addWidget(buttonHide);
	//layout->addWidget(buttonShow);
	layout->addWidget(checkBoxPass);
	checkBoxPass->setCheckState(Qt::Checked);
	alignment->setFilterPass(true);
	scaleButtonClicked(true);
	buttonHide->setChecked(true);
	connect(buttonGroupShow, SIGNAL(buttonClicked(int)), this, SLOT(showButtonChanged(int)));
	connect(checkBoxPass, SIGNAL(stateChanged(int)), this, SLOT(checkBoxPassChanged(int)));
	
	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(false);
	
	for ( int i = 0; i < alignment->getFilterCount(); i++ )
	{
		QCheckBox *checkBox = new QCheckBox(alignment->getFilter(i).name);
		checkBox->setToolTip(alignment->getFilter(i).description);
		layout->addWidget(checkBox);
		buttonGroup->addButton(checkBox);
		buttonGroup->setId(checkBox, i);
	}
	
	layout->addWidget(buttonScale);
	layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	setLayout(layout);
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(checkBoxChanged(int)));
	connect(buttonScale, SIGNAL(clicked(bool)), this, SLOT(scaleButtonClicked(bool)));
	
	adjustSize();
	setWindowTitle("Variants");
}

void FilterControl::checkBoxChanged(int id)
{
	if ( ((QCheckBox *)(buttonGroup->button(id)))->checkState() == Qt::Checked )
	{
		alignment->enableFilter(id);
	}
	else if ( ((QCheckBox *)(buttonGroup->button(id)))->checkState() == Qt::Unchecked )
	{
		alignment->disableFilter(id);
	}
	
	emit filtersChanged();
}

void FilterControl::checkBoxPassChanged(int state)
{
	alignment->setFilterPass(state == Qt::Checked);
	emit filtersChanged();
}

void FilterControl::showButtonChanged(int id)
{
	alignment->setFilterShow(id);
	emit filtersChanged();
}

void FilterControl::scaleButtonClicked(bool)
{
	alignment->setFilterScale();
	emit filtersChanged();
}

