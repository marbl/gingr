// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__AnnotationView__
#define __gavqt__AnnotationView__

#include "DrawingArea.h"
#include "Alignment.h"
#include <QVector>
#include <QDomDocument>
#include "harvest/HarvestIO.h"

struct FeatureView
{
	int start;
	int end;
	long long int startUngapped;
	long long int endUngapped;
	int row;
	QString name;
	QString locus;
	QString description;
	QString feature;
	QColor color;
	bool rc;
	bool search;
};

class AnnotationView : public DrawingArea
{
	Q_OBJECT
	
public:
	
	AnnotationView(QWidget *parent = 0);
	~AnnotationView();
	
	void clear();
	void load(const AnnotationList & annotationList, const Alignment * alignment);
	void setAlignment(const Alignment * newAlignment);
	void setPosition(int gapped, int ungapped, int offset);
	void setWindow(int newStart, int newEnd);
	void update();
	
public slots:
	
	void search(const QString & string, bool matchCase);
	
signals:
	
	void positionChanged(int ungapped);
	void signalMouseWheel(int delta);
	void signalSearchResults(int results) const;
	void signalWindowTarget(int start, int end);
	
protected:
	
	bool event(QEvent * event);
	void leaveEvent(QEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent *event);
	void updateBuffer();
	void wheelEvent(QWheelEvent * event);
	
private:
	
	void checkHighlight();
	void drawAnnotation(int index, QPainter * painter, bool highlight = false);
	void drawAnnotationLines(int index, QPainter * painter);
	void drawContigs(QPainter * painter);
	void drawHistogram(QPainter * painter);
	void drawLcbs(QPainter * painter);
	void renewHistogram();
	void setAnnotationRange();
	void setRows();
	void updatePosition();
	
	const Alignment * alignment;
	QVector<FeatureView> annotations;
	long long int start;
	long long int end;
	int annotationStart;
	int annotationEnd;
	int sequenceStart;
	int sequenceEnd;
	int rows;
	int rowsCur;
	bool updateNeeded;
	int position;
	int * histogram;
	int ** annotationIndeces;
	int highlightAnnotation;
	int focusAnn;
	int cursorX;
	int cursorY;
};

bool annotationLessThan(const FeatureView& a, const FeatureView& b);

#endif /* defined(__gavqt__AnnotationView__) */
