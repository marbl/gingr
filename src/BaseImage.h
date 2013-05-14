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

class BaseImage : public QImage
{
	
public:
	
	BaseImage(int width, int height, char base, bool snp);
	
private:
	
};

static QColor refA = qRgb(32, 48, 32);
static QColor refC = qRgb(32, 32, 48);
static QColor refG = qRgb(48, 44, 32);
static QColor refT = qRgb(48, 32, 32);
static QColor refN = qRgb(32, 32, 32);
static QColor ref_ = qRgb(64, 64, 64);

static QColor snpA = qRgb(0, 255, 0);
static QColor snpC = qRgb(0, 192, 255);
static QColor snpG = qRgb(255, 192, 0);
static QColor snpT = qRgb(255, 0, 0);
static QColor snpN = qRgb(240, 240, 240);
static QColor snp_ = qRgb(240, 240, 240);

static QColor refFontA = qRgb(80, 96, 80);
static QColor refFontC = qRgb(96, 96, 128);
static QColor refFontG = qRgb(128, 108, 96);
static QColor refFontT = qRgb(128, 96, 96);
static QColor refFontN = qRgb(96, 96, 96);
static QColor refFont_ = qRgb(128, 128, 128);

static QColor snpFontA = qRgb(32, 64, 32);
static QColor snpFontC = qRgb(32, 32, 64);
static QColor snpFontG = qRgb(64, 48, 32);
static QColor snpFontT = qRgb(64, 32, 32);
static QColor snpFontN = qRgb(128, 128, 128);
static QColor snpFont_ = qRgb(128, 128, 128);

#endif /* defined(__gavqt__BaseImage__) */
