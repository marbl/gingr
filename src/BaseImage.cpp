// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "BaseImage.h"
#include <QPainter>

BaseImage::BaseImage(int width, int height, char base, bool light, bool snp, bool gap, bool legend)
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
	
	float shadeBg;
	
	if ( width > 3 )
	{
		shadeBg = 1;
	}
	else
	{
		shadeBg = width / 4.;
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
	
	if ( gap && base == '-' )
	{
		color = colorShowGap;
		colorFont = colorShowGapFont;
	}
	else if ( light )
	{
		if ( snp )
		{
			color = colorLightSnp[index];
			colorFont = colorLightSnpFont[index];
		}
		else
		{
			if ( legend )
			{
				color = colorLightRefLegend[index];
				colorFont = colorLightRefLegendFont[index];
			}
			else
			{
				color = colorLightRef[index];
				colorFont = colorLightRefFont[index];
			}
		}
		
		if ( ! snp )
		{
			int offset = 255 * (1 - shadeBg);
			color = qRgb(offset + color.red() * shadeBg, offset + color.green() * shadeBg, offset + color.blue() * shadeBg);
		}
	}
	else
	{
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
		
		if ( ! snp )
		{
			color = qRgb(color.red() * shadeBg, color.green() * shadeBg, color.blue() * shadeBg);
		}
	}
	
	fill(color);
	painter.setPen(QColor::fromRgba(qRgba(colorFont.red(), colorFont.green(), colorFont.blue(), shade)));
	painter.setFont(QFont(painter.font().family(), size - 1));
	painter.setBackground(color);
	painter.drawText(0, 2, QPixmap::width(), QPixmap::height() - 2, Qt::AlignCenter | Qt::AlignVCenter, QString("%1").arg(base));
	//	painter.drawText(0, (image.height() - 2 * size / 3) / 2, image.width(), 2 * size / 3, Qt::AlignCenter, QString("%1").arg(chr));
}