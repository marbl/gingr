// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef gavqt_RegionView_h
#define gavqt_RegionView_h

#include <QColor>

class RegionView
{
public:
	
//	float getEnd() const;
//	float getStart() const;
//	bool getRc() const;
	RegionView();
	RegionView(int newLcb, float newStart, float newEnd, bool newRc, float newStartRef, float newEndRef, bool newRcRef);
	
	bool contains(int position, int width) const;
	void draw(QRgb * scanLine, const QRgb * palette, const int * hues, int width, bool rcTrack, int offset, float scale, int partition) const;
	void drawHighlight(QRgb *scanLine, int width, int offset, float hlOffset, bool rcTrack, float scale, int partition) const;
	int getLcb() const;
	bool getRc() const;
	float offsetRegion(int position, int width) const;
	float offsetTrack(float offsetRegion) const;
	bool operator<(const RegionView& other) const;
	
protected:
	
	float end;
	float endRef;
	int lcb; // TODO: this is bogus if regions are added
	bool rc;
	bool rcRef;
	float start;
	float startRef;
};

inline RegionView::RegionView() {}
inline RegionView::RegionView(int newLcb, float newStart, float newEnd, bool newRc, float newStartRef, float newEndRef, bool newRcRef)
{
	lcb = newLcb;
	start = newStart;
	end = newEnd;
	rc = newRc;
	startRef = newStartRef;
	endRef = newEndRef;
	rcRef = newRcRef;
}
inline bool RegionView::contains(int position, int width) const {return position >= (int)(start * width) && position <= (int)(end * width);}
inline int RegionView::getLcb() const {return lcb;}
inline bool RegionView::getRc() const {return rc;}
inline float RegionView::offsetRegion(int position, int width) const {return (float)(position - (int)(start * width)) / width;}
inline float RegionView::offsetTrack(float offsetRegion) const {return start + offsetRegion;}
inline bool RegionView::operator<(const RegionView &other) const {return end - start > other.end - other.start;}

float wrap(float value, float min, float max);

#endif
