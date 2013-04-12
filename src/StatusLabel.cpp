//
//  StatusLabel.cpp
//  gavqt
//
//  Created by Brian Ondov on 4/4/13.
//
//

#include "StatusLabel.h"
#include <QHBoxLayout>

StatusLabel::StatusLabel(QWidget * parent)
: QFrame(parent)
{
	setFrameStyle(QFrame::Panel | QFrame::Sunken);
	setLineWidth(1);
	setStyleSheet("* { background-color: rgb(245, 245, 245); }");
	label = new QLabel();
	QFont font = label->font();
	font.setPixelSize(11);
	label->setFont(font);
	label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	QHBoxLayout * layout = new QHBoxLayout();
	layout->addWidget(label);
	layout->setMargin(0);
	layout->setContentsMargins(2, 0, 0, 0);
	setLayout(layout);
	
	setStyleSheet("* { color: rgb(80, 80, 80); }");
}

void StatusLabel::setColorBG(QColor color)
{
	setStyleSheet(QString("* { background-color: rgb(%1, %2, %3); }").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void StatusLabel::setColorFG(QColor color)
{
	setStyleSheet(QString("* { color: rgb(%1, %2, %3); }").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void StatusLabel::setText(const QString &text)
{
	label->setText(text);
}
