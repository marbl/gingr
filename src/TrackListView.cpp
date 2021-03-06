// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "TrackListView.h"
#include <QMouseEvent>


TrackListView::TrackListView()
{
	cursorX = -1;
	cursorY = -1;
	trackCursor = -1;
	trackHover = -1;
	trackFocus = -1;
	setMouseTracking(true);
	updateNeeded = true;
	trackHeights = 0;
	trackCount = 0;
	zoomProgress = 0;
}

void TrackListView::handleTrackHeightChange(const TrackListView * focus)
{
	if ( this == focus )
	{
		updateTrackCursor();
	}
	
	setBufferUpdateNeeded();
	setUpdateNeeded();
}

void TrackListView::setIdByTrack(const std::vector<int> * newIdByTrack)
{
	idByTrack = newIdByTrack;
	
	trackById.resize(idByTrack->size());
	
	for ( int i = 0; i < idByTrack->size(); i++ )
	{
		trackById[(*idByTrack)[i]] = i;
	}
}

void TrackListView::setTrackFocus(int track)
{
	trackFocus = track;
}

void TrackListView::setTrackHeights(float *heights, int tracks)
{
	trackHeights = heights;
	trackCount = tracks;
}

void TrackListView::setTrackHover(int track, int trackEnd)
{
	trackHover = track;
	trackHoverEnd = trackEnd;
	setUpdateNeeded();
}

void TrackListView::setZoomProgress(float progress)
{
	bool updateCursor = zoomProgress != progress && progress == 1;
	
	zoomProgress = progress;
	
	if ( updateCursor )
	{
		updateTrackCursor();
	}
	
	setUpdateNeeded();
	setBufferUpdateNeeded();
}

void TrackListView::update()
{
	if ( updateNeeded )
	{
		DrawingArea::update();
		updateNeeded = false;
	}
}

void TrackListView::mouseMoveEvent(QMouseEvent * event)
{
	cursorX = event->pos().x() - frameWidth();
	cursorY = event->pos().y() - frameWidth();
	
	//signalFocus(this);
	updateTrackCursor();
}

void TrackListView::updateTrackCursor()
{
	if ( trackCount == 0 || cursorY == -1 )
	{
		return;
	}
	
	int trackCursorPrev = trackCursor;
	trackCursor = 0;
	
	while ( trackHeights[trackCursor + 1] < cursorY )
	{
		trackCursor++;
	}
	
	if ( trackCursor >= trackCount )
	{
		trackCursor = -1;
	}
	
	if ( trackCursor != trackCursorPrev )
	{
		handleTrackCursorChange();
	}
}

void TrackListView::enterEvent(QEvent *)
{
	signalFocus(this);
}

void TrackListView::handleTrackCursorChange()
{
	signalTrackHoverChange(trackCursor, trackCursor);
}

void TrackListView::leaveEvent(QEvent *)
{
	cursorX = -1;
	cursorY = -1;
	trackCursor = -1;
	signalTrackHoverChange(-1, -1);
	signalUnfocus(this);
}

