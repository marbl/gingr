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

static QColor colorCoreBad = qRgb(240, 200, 200);
static QColor colorCoreOk = qRgb(240, 240, 200);
static QColor colorCoreGood = qRgb(200, 245, 200);
static QColor colorGapBG = qRgb(245, 245, 245);
static QColor colorGapFG = qRgb(80, 80, 80);
static QColor colorGapShowBG = qRgb(128, 224, 224);
static QColor colorGapShowFG = qRgb(64, 96, 96);
static QColor colorGapDisabledBG = qRgb(245, 245, 245);
static QColor colorGapDisabledFG = qRgb(160, 160, 160);
static QColor colorGapShowDisabledBG = qRgb(160, 160, 160);
static QColor colorGapShowDisabledFG = qRgb(224, 224, 224);

class BlockStatusBar : public QWidget
{
public:
	
	BlockStatusBar();
	
	const StatusLabel * getLabelGaps() const;
	const StatusLabel * getLabelIns() const;
	const StatusLabel * getLabelDel() const;
	void setCore(float core);
	void setLegendBases(bool bases);
	void setLightColors(bool light);
	void setPosition(int ungapped, int offset);
	void setShowGaps(bool gaps);
	void setShowIns(bool ins);
	void setShowDel(bool del);
	void setShowLegend(bool show);
	void setSynteny(bool synteny);
	
private:
	
	void updateColors();
	
	SnpLegend * snpLegend;
	StatusLabel * labelPosition;
	StatusLabel * labelOffset;
	StatusLabel * labelName;
	StatusLabel * labelGaps;
	StatusLabel * labelIns;
	StatusLabel * labelDel;
};

inline const StatusLabel * BlockStatusBar::getLabelGaps() const {return labelGaps;}
inline const StatusLabel * BlockStatusBar::getLabelIns() const {return labelIns;}
inline const StatusLabel * BlockStatusBar::getLabelDel() const {return labelDel;}
inline void BlockStatusBar::setLightColors(bool light) {snpLegend->setLightColors(light);}
inline void BlockStatusBar::setShowLegend(bool show) {snpLegend->setShow(show);}

#endif /* defined(__gavqt__BlockStatusBar__) */
