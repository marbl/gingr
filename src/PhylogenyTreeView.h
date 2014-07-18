// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef gavqt_PhylogenyTreeView_h
#define gavqt_PhylogenyTreeView_h

#include "TrackListView.h"
#include "harvest/PhylogenyTree.h"
#include "Track.h"
#include "Tween.h"
#include <QPainter>
#include <QVector>

class PhylogenyTreeView : public TrackListView
{
	Q_OBJECT
	
public:
	
	PhylogenyTreeView();
	~PhylogenyTreeView();
	
	void clear();
	void handleTrackHeightChange(const TrackListView * focus);
	void setNames(QVector<QString> * newNames);
	void setPhylogenyTree(const PhylogenyTree * newTree);
	void setRightAlignNodes(bool align);
	void setRightAlignText(bool align);
	void setTrackFocus(int track);
	void setTrackHover(int track, int trackEnd);
	void setTrackReference(int track);
	
protected:
	
	class PhylogenyTreeNodeView
	{
		Tween tweenX;
		Tween tweenXAlign;
		
	public:
		int y;
		int group;
		bool search;
		void setTargetX(int x, bool initialize = false) {tweenX.setTarget(x, initialize);}
		void setTargetXAlign(int xAlign, bool initialize = false) {tweenXAlign.setTarget(xAlign, initialize);}
		void update(float progress) {tweenX.update(progress);tweenXAlign.update(progress);}
		int getX() const {return tweenX.getValue();}
		int getXAlign() const {return tweenXAlign.getValue();}
		int getY() const {return y;}
	};
	
	virtual float getHighlight(const PhylogenyTreeNode *, float highlight, bool) const;
	void groupNodes(const PhylogenyTreeNode * node);
	virtual QColor highlightColor(float highlight) const = 0;
	void leaveEvent(QEvent * event);
	virtual bool nodeIsVisible(const PhylogenyTreeNode * node, float leafSize) const = 0;
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent * event);
	void setWindow(const PhylogenyTreeNode * node, bool initialize = false);
	void updateBuffer();
	
	QVector<QString> * names;
	const PhylogenyTree * phylogenyTree;
	const PhylogenyTreeNode * highlightNode;
	const PhylogenyTreeNode * focusNode;
	PhylogenyTreeNodeView * nodeViews;
	bool zoomIn;
	bool rightAlign;
	bool rightAlignLast;
	bool rightAlignText;
	bool useGradient;
	bool leafLines;
	bool showDots;
	int trackReference;
	
signals:
	
	void signalNodeHover(const PhylogenyTreeNode * node);
	void signalTrackZoom(int start, int end);
	
private:
	
	void drawLine(QPainter * painter, float x1, float y1, float x2, float y2, float weight, QColor color, bool dash = false) const;
	void drawLabel(QPainter * painter, int leaf, int x, float highlight, bool search) const;
	void drawLabels(QPainter * painter, bool drawHighlight) const;
	void drawNode(QPainter * painter, const PhylogenyTreeNode * node, bool drawHighlight, float highlight = 0, int xLeft = 0, float weightTop = 1, float weightBottom = 1, int group = 0) const;
	void drawTrackFocus(QPainter * painter);
	void fillGradient(QPixmap * pixmap, QColor colorStart, QColor colorEnd);
	void drawNodeLeaf(QPainter * painter, const PhylogenyTreeNode * node, float highlight, float weightTop, float weightBottom) const;
	float getNodeDepth(const PhylogenyTreeNode * node) const;
	float getNodeAlignDist(const PhylogenyTreeNode * node) const;
	QColor gradient(int x) const;
	float maxVisibleDepth(const PhylogenyTreeNode * node, float leafSize) const;
	float nameScale(int leaf) const;
	void setTargetsNodeView(const PhylogenyTreeNode * node, bool initialize = false);
	void updateNodeViews(const PhylogenyTreeNode * node);
	float zoomLerp(float start, float end) const;
	
	static const int colorCount = 2;
//	static const QColor colorHover = qRgb()
	int radius;
	Tween tweenAlign;
	QPen penDark;
	QPen penLight;
//	QImage * buffer;
	QImage ** nameBuffers;
	QPixmap * gradients[colorCount];
	QPixmap * gradientsHover[colorCount];
	QPixmap * gradientsHighlight[colorCount];
	QPixmap * gradientsSearch[colorCount];
	QColor colors[colorCount];
	int nameBufferCount;
	float xFactor;
	float xFactorStart;
	float xFactorEnd;
	float xOffset;
	float xOffsetStart;
	float xOffsetEnd;
	bool redrawNeeded;
	bool bufferHighlight;
	int fontHeight;
	float * leafHues;
};

inline void PhylogenyTreeView::setRightAlignText(bool align) {rightAlignText = align;setBufferUpdateNeeded();}

inline float maxf(float a, float b) {return a > b ? a : b;}
inline float minf(float a, float b) {return a < b ? a : b;}

#endif
