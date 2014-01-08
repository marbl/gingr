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
//	setStyleSheet("* { background-color: rgb(245, 245, 5); }");
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

void StatusLabel::setAlignment(Qt::Alignment alignment)
{
	label->setAlignment(alignment);
}

void StatusLabel::setColorBG(QColor color)
{
	QString style = QString("QFrame { background-color: rgb(%1, %2, %3); }").arg(color.red()).arg(color.green()).arg(color.blue());
	
	setStyleSheet(style);
	label->setStyleSheet(style);
}

void StatusLabel::setColorFG(QColor color)
{
	label->setStyleSheet(QString("* { color: rgb(%1, %2, %3); }").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void StatusLabel::setText(const QString &text)
{
	label->setText(text);
}

void StatusLabel::setWidth(int width)
{
	setMinimumWidth(width);
	setMaximumHeight(width);
}

void StatusLabel::mousePressEvent(QMouseEvent *)
{
	emit clicked();
}
