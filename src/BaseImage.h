//
//  BaseImage.h
//  gavqt
//
//  Created by Brian Ondov on 5/3/13.
//
//

#ifndef __gavqt__BaseImage__
#define __gavqt__BaseImage__

#include <QImage>
#include <QColor>
#include <QPixmap>

class BaseImage : public QPixmap
{
	
public:
	
	BaseImage(int width, int height, char base, bool light, bool snp, bool gap, bool legend = false);
	
private:
	
};

static QColor colorRef[6] =
{
	qRgb(32, 64, 32),
	qRgb(32, 32, 64),
	qRgb(64, 48, 32),
	qRgb(64, 32, 32),
	qRgb(64, 32, 64),
	qRgb(112, 112, 112),
};

static QColor colorRefLegend[6] =
{
	qRgb(64, 96, 64),
	qRgb(64, 64, 96),
	qRgb(96, 80, 48),
	qRgb(112, 64, 64),
	qRgb(112, 64, 112),
	qRgb(112, 112, 112),
};

static QColor colorSnp[6] =
{
	qRgb(0, 255, 0),
	qRgb(0, 192, 255),
	qRgb(255, 192, 0),
	qRgb(255, 0, 0),
	qRgb(255, 0, 255),
	qRgb(224, 224, 224),
};

static QColor colorRefFont[6] =
{
	qRgb(80, 112, 80),
	qRgb(96, 96, 144),
	qRgb(144, 112, 96),
	qRgb(144, 96, 96),
	qRgb(144, 64, 144),
	qRgb(196, 196, 196),
};

static QColor colorRefLegendFont[6] =
{
	qRgb(128, 196, 128),
	qRgb(160, 160, 255),
	qRgb(196, 160, 128),
	qRgb(216, 128, 128),
	qRgb(216, 128, 216),
	qRgb(196, 196, 196),
};

static QColor colorSnpFont[6] =
{
	qRgb(16, 96, 16),
	qRgb(64, 64, 96),
	qRgb(96, 80, 32),
	qRgb(96, 32, 32),
	qRgb(112, 0, 112),
	qRgb(96, 96, 96),
};

static QColor colorLightRef[6] =
{
	qRgb(224, 255, 224),
	qRgb(224, 224, 255),
	qRgb(255, 255, 224),
	qRgb(255, 224, 224),
	qRgb(255, 224, 255),
	qRgb(255, 255, 255),
	//	qRgb(64, 64, 64),
};

static QColor colorLightRefLegend[6] =
{
	qRgb(224, 255, 224),
	qRgb(224, 224, 255),
	qRgb(255, 255, 224),
	qRgb(255, 224, 224),
	qRgb(255, 224, 255),
	qRgb(255, 255, 255),
	//	qRgb(96, 96, 96),
};

static QColor colorLightSnp[6] =
{
	qRgb(80, 192, 80),
	qRgb(80, 80, 192),
	qRgb(192, 128, 80),
	qRgb(192, 80, 80),
	qRgb(192, 80, 192),
	qRgb(112, 112, 112),
};

static QColor colorLightRefFont[6] =
{
	qRgb(128, 192, 128),
	qRgb(128, 128, 192),
	qRgb(192, 144, 128),
	qRgb(192, 128, 128),
	qRgb(192, 128, 192),
	qRgb(128, 128, 128),
	//	qRgb(144, 144, 144),
};

static QColor colorLightRefLegendFont[6] =
{
	qRgb(80, 112, 80),
	qRgb(96, 96, 144),
	qRgb(144, 112, 96),
	qRgb(144, 96, 96),
	qRgb(144, 64, 144),
	qRgb(112, 112, 112),
	//	qRgb(196, 196, 196),
};

static QColor colorLightSnpFont[6] =
{
	qRgb(224, 255, 224),
	qRgb(224, 224, 255),
	qRgb(255, 255, 224),
	qRgb(255, 224, 224),
	qRgb(255, 224, 255),
	qRgb(255, 255, 255),
};

static QColor colorShowGap = qRgb(128, 224, 224);
static QColor colorShowGapFont = qRgb(64, 96, 96);

#endif /* defined(__gavqt__BaseImage__) */
