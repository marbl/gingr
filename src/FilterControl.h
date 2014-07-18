// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__FilterControl__
#define __gavqt__FilterControl__

#include "OptionPanel.h"
#include "Alignment.h"
#include <QButtonGroup>

class FilterControl : public OptionPanel
{
	Q_OBJECT
	
public:
	
	FilterControl(QWidget * parent = 0);
	void setAlignment(Alignment * newAlignment);
	
public slots:
	
	void checkBoxChanged(int id);
	void checkBoxPassChanged(int state);
	void showButtonChanged(int id);
	void scaleButtonClicked(bool);
	
signals:
	
	void filtersChanged();
	
private:
	
	Alignment * alignment;
	QButtonGroup * buttonGroup;
};

#endif /* defined(__gavqt__FilterControl__) */
