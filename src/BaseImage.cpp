//
//  BaseImage.cpp
//  gavqt
//
//  Created by Brian Ondov on 5/3/13.
//
//

#include "BaseImage.h"
#include <QPainter>

BaseImage::BaseImage(int width, int height, char base, bool snp)
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
	
	if ( size > 6 )
	{
		shade = 255;
	}
	else
	{
		shade = size * 255 / 6;
	}
	
	QColor color;
	QColor colorFont;
	
	if ( snp )
	{
		switch ( base )
		{
			case 'A':
				color = snpA;
				colorFont = snpFontA;
				break;
			case 'C':
				color = snpC;
				colorFont = snpFontC;
				break;
			case 'G':
				color = snpG;
				colorFont = snpFontG;
				break;
			case 'N':
				color = snpN;
				colorFont = snpFontN;
				break;
			case 'T':
				color = snpT;
				colorFont = snpFontT;
				break;
			case '-':
				color = snp_;
				colorFont = snpFont_;
				break;
		}
	}
	else
	{
		switch ( base )
		{
			case 'A':
				color = refA;
				colorFont = refFontA;
				break;
			case 'C':
				color = refC;
				colorFont = refFontC;
				break;
			case 'G':
				color = refG;
				colorFont = refFontG;
				break;
			case 'N':
				color = refN;
				colorFont = refFontN;
				break;
			case 'T':
				color = refT;
				colorFont = refFontT;
				break;
			case '-':
				color = ref_;
				colorFont = refFont_;
				break;
		}
	}
	
	fill(color);
	painter.setPen(QColor::fromRgba(qRgba(colorFont.red(), colorFont.green(), colorFont.blue(), shade)));
	painter.setFont(QFont(painter.font().family(), size));
	painter.setBackground(color);
	painter.drawText(0, 1, QPixmap::width(), QPixmap::height() - 1, Qt::AlignCenter | Qt::AlignVCenter, QString("%1").arg(base));
	//	painter.drawText(0, (image.height() - 2 * size / 3) / 2, image.width(), 2 * size / 3, Qt::AlignCenter, QString("%1").arg(chr));
}