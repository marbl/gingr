//
//  TrackListView.h
//  gavqt
//
//  Created by Brian Ondov on 10/31/12.
//
//

#ifndef __gavqt__TrackListView__
#define __gavqt__TrackListView__

#include "DrawingArea.h"
#include <QVector>

class TrackListView : public DrawingArea
{
	Q_OBJECT
	
public:
	
	TrackListView();
	
	virtual void handleTrackHeightChange(const TrackListView * focus);
	void setIdByTrack(const QVector<int> * newIdByTrack);
	void setTrackFocus(int track);
	void setTrackHeights(float * heights, int tracks);
	void setTrackHover(int track, int trackEnd);
	void setZoomProgress(float progress);
	void update();
	
protected:
	
	void enterEvent(QEvent * event);
	int getCursorX() const;
	int getCursorY() const;
	int getIdByTrack(int id) const;
	int getTrackCount() const;
	int getTrackCursor() const;
	int getTrackFocus() const;
	float getTrackHeight(int track) const;
	int getTrackHover() const;
	int getTrackHoverEnd() const;
	int getTrackById(int track) const;
	float getZoomProgress() const;
	void handleTrackCursorChange();
	void leaveEvent(QEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void setUpdateNeeded();
	virtual void updateTrackCursor();
	void updateTrackFocus(int track);
	void updateTrackHover(int track);
	
signals:
	
	void signalFocus(TrackListView * view);
	void signalTrackFocusChange(int track);
	void signalTrackHoverChange(int track, int trackEnd);
	void signalUnfocus(TrackListView * view);
	
private:
	
	int cursorX;
	int cursorY;
	
	float * trackHeights;
	const QVector<int> * idByTrack;
	QVector<int> trackById;
	int trackCount;
	int trackCursor;
	int trackFocus;
	int trackHover;
	int trackHoverEnd;
	float zoomProgress;
	
	bool updateNeeded;
};

inline int TrackListView::getCursorX() const {return cursorX;}
inline int TrackListView::getCursorY() const {return cursorY;}
inline int TrackListView::getTrackCount() const {return trackCount;}
inline int TrackListView::getTrackCursor() const {return trackCursor;}
inline int TrackListView::getTrackFocus() const {return trackFocus;}
inline float TrackListView::getTrackHeight(int track) const {return trackHeights[track];}
inline int TrackListView::getTrackHover() const {return trackHover;}
inline int TrackListView::getTrackHoverEnd() const {return trackHoverEnd;}
inline int TrackListView::getTrackById(int id) const {return trackById[id];}
inline int TrackListView::getIdByTrack(int track) const {return (*idByTrack)[track];}
inline float TrackListView::getZoomProgress() const {return zoomProgress;}
inline void TrackListView::setUpdateNeeded() {updateNeeded = true;}

#endif /* defined(__gavqt__TrackListView__) */
