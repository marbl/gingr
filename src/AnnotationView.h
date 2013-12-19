//
//  AnnotationView.h
//  gavqt
//
//  Created by Brian Ondov on 2/4/13.
//
//

#ifndef __gavqt__AnnotationView__
#define __gavqt__AnnotationView__

#include "DrawingArea.h"
#include "Alignment.h"
#include <QVector>
#include <QDomDocument>
#include "HarvestIO.h"

struct Annotation
{
	int start;
	int end;
	long long int startUngapped;
	long long int endUngapped;
	int row;
	QString name;
	QString locus;
	QString description;
	QColor color;
	bool rc;
	const Annotation * nextByStart;
};

class AnnotationView : public DrawingArea
{
	Q_OBJECT
	
public:
	
	AnnotationView(QWidget *parent = 0);
	~AnnotationView();
	
	void loadDom(const QDomElement * element);
	void loadPb(const Harvest::AnnotationList & msg, const Harvest::Reference & msgRef);
	void setAlignment(const Alignment * newAlignment);
	void setPosition(int gapped, int ungapped, int offset);
	void setWindow(int newStart, int newEnd);
	void update();
	
signals:
	
	void positionChanged(int ungapped);
	void signalMouseWheel(int delta);
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
	void drawHistogram();
	void renewHistogram();
	void setAnnotationRange();
	void setRows(int newRows);
	void updatePosition();
	
	const Alignment * alignment;
	QVector<Annotation> annotations;
	long long int start;
	long long int end;
	int annotationStart;
	int annotationEnd;
	int sequenceStart;
	int sequenceEnd;
	int rows;
	bool updateNeeded;
	int position;
	int * histogram;
	int ** annotationIndeces;
	int highlightAnnotation;
	int focusAnn;
	int cursorX;
	int cursorY;
};

bool annotationLessThan(const Annotation& a, const Annotation& b);

#endif /* defined(__gavqt__AnnotationView__) */
