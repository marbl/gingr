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

struct Annotation
{
	int start;
	int end;
	int startUngapped;
	int endUngapped;
	int row;
	QString name;
	QColor color;
	bool rc;
	const Annotation * nextByStart;
};

class AnnotationView : public DrawingArea
{
	Q_OBJECT
	
public:
	
	AnnotationView(QWidget *parent = 0);
	void loadDom(const QDomElement * element);
	void setAlignment(const Alignment * newAlignment);
	void setPosition(int gapped, int ungapped, int offset);
	void setWindow(unsigned int newStart, unsigned int newEnd);
	void update();
	
protected:
	
	void paintEvent(QPaintEvent * event);
	void updateBuffer();
	
private:
	
	void drawAnnotation(int index, QPainter * painter);
	void drawAnnotationLines(int index, QPainter * painter);
	void setAnnotationRange();
	void setRows(int newRows);
	
	const Alignment * alignment;
	QVector<Annotation> annotations;
	int start;
	int end;
	int annotationStart;
	int annotationEnd;
	int rows;
	bool updateNeeded;
	int position;
};

bool annotationLessThan(const Annotation& a, const Annotation& b);

#endif /* defined(__gavqt__AnnotationView__) */
