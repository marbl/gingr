// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "TrackView.h"

float absf(float value);
int floor(float value);

TrackView::TrackView()
{
	regionViewsByLcb = 0;
	rc = false;
	orientation.initialize(1, 1);
}

TrackView::~TrackView()
{
	if ( regionViewsByLcb )
	{
		delete [] regionViewsByLcb;
	}
}

void TrackView::alignLcb(int lcb, float lcbOffset, float offset, bool lcbRc)
{
	float offsetLcb = regionViewsByLcb[lcb]->offsetTrack(lcbOffset);
	bool rcPrev = rc;
	int factor;
	
	rc = lcbRc != regionViewsByLcb[lcb]->getRc();
	
	if ( rc )
	{
		offsetLcb = -offsetLcb;
	}
	else
	{
	}
	
	if ( rc != rcPrev )
	{
		factor = -1;
		orientation.initialize(-1, 1);
	}
	else
	{
		factor = 1;
		orientation.setTarget(1);
	}
	
	float offsetXNew = wrap(offset - offsetLcb, 0, 1);
	float offsetXOld;
	
	offsetXOld = wrap(offsetX.getValue(), 0, 1);
	
	offsetXOld += floor(offsetXNew + offsetLcb) - floor(offsetXOld + offsetLcb * factor);
	
	float pivot = wrap((offsetXOld + offsetXNew) / 2, 0, 1);
	
	if ( pivot > .5 )
	{
		partition = 2 * pivot - 1;
	}
	else
	{
		partition = 2 * pivot;
	}
	
	if ( rc != rcPrev )
	{
		offsetXOld = offsetXNew;
	}
	
	//printf("%f, %f\n", offsetXOld, offsetXNew);
	offsetX.initialize(offsetXOld, offsetXNew);
}

void TrackView::draw(QImage * image, QRgb * const * palette, int * hues, float progress, float offsetY, float height, bool hl, int hlLcb, float hlOffset)
{
	int y = offsetY;// * image->height();
	QRgb * line = (QRgb *)image->scanLine(y);
	
	offsetX.update(progress);
	orientation.update(progress);
	
	int offset = wrap(offsetX.getValue(), 0, 1) * image->width(); // TODO: no wrap
	int partitionScaled = partition * image->width();
	QRgb * paletteShade = palette[(int)((orientation.getValue() + 1) * 63 / 2)];
	
	for( int i = 0; i < regionViews.size(); i++ )
	{
		regionViews[i]->draw(line, paletteShade, hues, image->width(), rc, offset, orientation.getValue(), partitionScaled);
	}
	
	if ( hl && regionViewsByLcb[hlLcb] )
	{
		regionViewsByLcb[hlLcb]->drawHighlight(line, image->width(), offset, hlOffset, rc, orientation.getValue(), partitionScaled);
	}
	
	for ( int i = y + 1; i <= (offsetY + height) * 1 /*image->height()*/ && i < image->height(); i++ )
	{
		memcpy(image->scanLine(i), line, sizeof(char) * 4 * image->width());
	}
}

int TrackView::getLcb(int position, int width, float &offset) const
{
	position -= offsetX.getValue() * width;
	
	if ( position < 0 )
	{
		position += width;
	}
	
	if ( rc )
	{
		position = width - position;
	}
	
//	for (int i = regionViews.size() - 1; i >= 0; i--)
	for( int i = 0; i < regionViews.size(); i++ )
	{
		if ( regionViews[i]->contains(position, width) )
		{
			offset = regionViews[i]->offsetRegion(position, width);
			return regionViews[i]->getLcb();
		}
	}
	
	//printf("ERROR\n");
	// TODO: error
}

float TrackView::getLcbOffset(int lcb, float offset) const
{
	return offsetX.getValue() + regionViewsByLcb[lcb]->offsetTrack(offset) * (rc ? -1 : 1);
}

void TrackView::initialize(const RegionVector * refByLcb, const RegionVector * newTrack, unsigned int refSize, float newOffsetY, float newHeight)
{
	int regionRefIndexLast;
	int lcb; // TODO: this is only valid if blocks are single regions (see below)
	unsigned int regionStart = 0;
	unsigned int blockStart = 0;
	unsigned int blockLength = 0;
	unsigned int blockStartRef = 0;
	unsigned int blockLengthRef = 0;
	bool blockRc;
	bool blockRcRef;
	unsigned int size = 0;
	const gav::Region * region;
	const gav::Region * regionRef;
	
	track = newTrack;
//	height = newHeight;
//	offsetY = newOffsetY;
	
	regionViewsByLcb = new const RegionView * [track->size()];
	
	for (int i = 0; i < track->size(); i++)
	{
		size += (*track)[i]->getLength();
	}
	
	regionViews.empty();
	
	for (int i = 0; i <= track->size(); i++ )
	{
		int regionRefIndex;
		
		if ( i < track->size() )
		{
			region = (*track)[i];
			regionRef = (*refByLcb)[(*track)[i]->getLcb()];
			regionRefIndex = regionRef->getLcb();
			
			if ( i == 0 )
			{
				blockStartRef = regionRef->getStart();
			}
		}
		
		if
		(
			i == track->size() ||
			(
				i > 0 &&
				(	true || // TODO: add regions to RegionView?
					regionRefIndex != regionRefIndexLast % track->size() + 1 ||
					(blockRc == blockRcRef) != (region->getRc() == regionRef->getRc())
				)
			)
		)
		{
			regionViews.push_back
			(
				new RegionView
				(
					lcb,
					(float)blockStart / size,
					(float)(blockStart + blockLength) / size,
					blockRc,
					(float)blockStartRef / refSize,
					(float)(blockStartRef + blockLengthRef) / refSize,
					blockRcRef
				)
			);
			
			regionViewsByLcb[lcb] = regionViews.last(); // TEMP
			
			if ( i < track->size() )
			{
//				blockStart = region->getStart();
				blockStart = regionStart;
				blockLength = 0;
				blockStartRef = regionRef->getStart();
				blockLengthRef = 0;
			}
		}
		
		if ( i < track->size() )
		{
			lcb = region->getLcb();
			regionStart += region->getLength();
			blockLength += region->getLength();
			blockLengthRef += regionRef->getLength();
			blockRc = region->getRc();
			blockRcRef = regionRef->getRc();
			regionRefIndexLast = regionRefIndex;
		}
	}
	
	trackLength = regionStart;
	qSort(regionViews.begin(), regionViews.end());
}

float absf(float value)
{
	if ( value < 0 )
	{
		value = -value;
	}
	
	return value;
}

int floor(float value)
{
	int result = value;
	
	if ( result > value )
	{
		result--;
	}
	
	return result;
}
