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
	void setShowLegend(bool show);
	void setSynteny(bool synteny);
	void setPosition(int ungapped, int offset);
	
private:
	
	SnpLegend * snpLegend;
	StatusLabel * labelPosition;
	StatusLabel * labelOffset;
	StatusLabel * labelName;
};

inline void BlockStatusBar::setShowLegend(bool show) {snpLegend->setShow(show);}
inline void BlockStatusBar::setLightColors(bool light) {snpLegend->setLightColors(light);}
#endif /* defined(__gavqt__BlockStatusBar__) */
