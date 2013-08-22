//
//  BaseImage.cpp
//  gavqt
//
//  Created by Brian Ondov on 5/3/13.
//
//

#include "BaseImage.h"
#include <QPainter>

BaseImage::BaseImage(int width, int height, char base, bool snp, bool legend)
: QPixmap(width + 1, height + 1)
{
	QPainter painter(this);
	
	int size;
	
	if ( width > height )
	{
		size = height;
	}
	else
	{
		size = width;
	}
	
	int shade;
	
	if ( size > 10 )
	{
		shade = 255;
	}
	else if ( size < 6 )
	{
		shade = 0;
	}
	else
	{
		shade = (size - 6) * 255 / 4;
	}
	
	QColor color;
	QColor colorFont;
	
	int index = 0;
	
	switch ( base )
	{
		case 'A': index = 0; break;
		case 'C': index = 1; break;
		case 'G': index = 2; break;
		case 'T': index = 3; break;
		case 'N': index = 4; break;
		case '-': index = 5; break;
	}
	
	if ( snp )
	{
		color = colorSnp[index];
		colorFont = colorSnpFont[index];
	}
	else
	{
		if ( legend )
		{
			color = colorRefLegend[index];
			colorFont = colorRefLegendFont[index];
		}
		else
		{
			color = colorRef[index];
			colorFont = colorRefFont[index];
		}
	}
	
	fill(color);
	painter.setPen(QColor::fromRgba(qRgba(colorFont.red(), colorFont.green(), colorFont.blue(), shade)));
	painter.setFont(QFont(painter.font().family(), size));
	painter.setBackground(color);
	painter.drawText(0, 1, QPixmap::width(), QPixmap::height() - 1, Qt::AlignCenter | Qt::AlignVCenter, QString("%1").arg(base));
	//	painter.drawText(0, (image.height() - 2 * size / 3) / 2, image.width(), 2 * size / 3, Qt::AlignCenter, QString("%1").arg(chr));
}