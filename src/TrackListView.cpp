//
//  TrackListView.cpp
//  gavqt
//
//  Created by Brian Ondov on 10/31/12.
//
//

#include "TrackListView.h"
#include <QMouseEvent>


TrackListView::TrackListView()
{
	trackCursor = -1;
	trackHover = -1;
	trackFocus = -1;
	setMouseTracking(true);
	updateNeeded = true;
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

void TrackListView::setIdByTrack(const QVector<int> * newIdByTrack)
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
	zoomProgress = progress;
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
	trackCursor = -1;
	signalTrackHoverChange(-1, -1);
	signalUnfocus(this);
}

