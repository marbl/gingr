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
#include "SnpLegend.h"

class BlockStatusBar : public QWidget
{
public:
	
	BlockStatusBar();
	
	void setLegendBases(bool bases);
	void setLightColors(bool light);
	void setPosition(int ungapped, int offset);
	void setShowGaps(bool gaps);
	void setShowLegend(bool show);
	void setSynteny(bool synteny);
	
private:
	
	SnpLegend * snpLegend;
	StatusLabel * labelPosition;
	StatusLabel * labelOffset;
	StatusLabel * labelName;
};

inline void BlockStatusBar::setLightColors(bool light) {snpLegend->setLightColors(light);}
inline void BlockStatusBar::setShowGaps(bool gaps) {snpLegend->setShowGaps(gaps);}
inline void BlockStatusBar::setShowLegend(bool show) {snpLegend->setShow(show);}

#endif /* defined(__gavqt__BlockStatusBar__) */
