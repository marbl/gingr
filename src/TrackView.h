// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef gavqt_TrackView_h
#define gavqt_TrackView_h

#include "Alignment.h"
#include <QImage>
#include "RegionView.h"
#include "Tween.h"

class TrackView
{
public:
	
	TrackView();
	~TrackView();
	void alignLcb(int lcb, float lcbOffset, float offset, bool lcbRc);
	void draw(QImage * image, QRgb * const * palette, int * hues, float progress, float offsetY, float height, bool hl, int hlLcb, float hlOffset);
	int getLcb(int position, int width, float & offset) const;
	float getLcbOffset(int lcb, float offset) const;
	bool getLcbRc(int lcb) const;
	float getOffset() const;
	bool getRc() const;
	void initialize(const RegionVector * refByLcb, const RegionVector * newTrack, unsigned int refSize, float newOffsetY, float newHeight);
	
private:
	
	Tween offsetX;
	Tween orientation;
	float partition;
	bool rc;
	QVector<RegionView *> regionViews;
	const RegionView ** regionViewsByLcb;
	const RegionVector * track;
	unsigned int trackLength;
};

inline bool TrackView::getLcbRc(int lcb) const {return regionViewsByLcb[lcb]->getRc();}
inline float TrackView::getOffset() const {return offsetX.getValue();}
inline bool TrackView::getRc() const {return rc;}

#endif
