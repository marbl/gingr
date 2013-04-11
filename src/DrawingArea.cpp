//
//  DrawingArea.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "DrawingArea.h"
#include <QPainter>

DrawingArea::DrawingArea(QWidget *parent)
: QFrame(parent)
{
	setFrameStyle(QFrame::Panel | QFrame::Sunken);
	setLineWidth(1);
	setStyleSheet("* { background-color: rgb(245, 245, 245); }");
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
	QFrame::paintEvent(event);
}
