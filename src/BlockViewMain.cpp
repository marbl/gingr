//
//  BlockViewMain.cpp
//  gavqt
//
//  Created by Brian Ondov on 3/29/13.
//
//

#include "BlockViewMain.h"
#include <QPainter>

BlockViewMain::BlockViewMain()
: BlockView()
{
	seq = 0;
}

BlockViewMain::~BlockViewMain()
{
	if ( seq )
	{
		for ( int i = 0; i < getTrackCount(); i++ )
		{
			delete [ ] seq[i];
		}
		
		delete [] seq;
	}
}

void BlockViewMain::updateBuffer()
{
	BlockView::updateBuffer();
	drawSequence();
}

void BlockViewMain::updateSnps()
{
	BlockView::updateSnps();
	
	if ( seq )
	{
		for ( int i = 0; i < getTrackCount(); i++ )
		{
			delete [ ] seq[i];
		}
		
		delete [] seq;
		seq = 0;
	}
	
	int windowSize = posEnd - posStart + 1;
	
	if ( windowSize < getWidth() )
	{
		seq = new char * [getTrackCount()];
		
		for ( int i = 0; i < getTrackCount(); i++ )
		{
			seq[i] = new char[windowSize];
			
			memcpy(seq[i], alignment->getRefSeqGapped() + posStart, windowSize);
		}
	}
}
void BlockViewMain::drawSequence() const
{
	if ( seq == 0 )
	{
		return;
	}
	
	int imageWidth = imageBuffer->width();
	float baseWidth = (float)imageWidth / (posEnd - posStart);
	
	QPainter painter(imageBuffer);
	
	if ( baseWidth < 1 )
	{
		return;
	}
	
	int fontSize = baseWidth;
	int trackHeight = getTrackHeight(1) - getTrackHeight(0);
	
	if ( fontSize > trackHeight )
	{
		fontSize = trackHeight;
	}
	
	QImage charA(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charC(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charG(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charN(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charT(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage char_(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	
	QImage charSnpA(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charSnpC(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charSnpG(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charSnpN(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charSnpT(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	QImage charSnp_(baseWidth + 1, trackHeight + 1, QImage::Format_RGB32);
	
	paintChar(charSnpA, 'A', fontSize, snpA, snpFontA);
	paintChar(charSnpC, 'C', fontSize, snpC, snpFontC);
	paintChar(charSnpG, 'G', fontSize, snpG, snpFontG);
	paintChar(charSnpN, 'N', fontSize, snpN, snpFontN);
	paintChar(charSnpT, 'T', fontSize, snpT, snpFontT);
	paintChar(charSnp_, '-', fontSize, snp_, snpFont_);
	
	paintChar(charA, 'A', fontSize, refA, refFontA);
	paintChar(charC, 'C', fontSize, refC, refFontC);
	paintChar(charG, 'G', fontSize, refG, refFontG);
	paintChar(charN, 'N', fontSize, refN, refFontN);
	paintChar(charT, 'T', fontSize, refT, refFontT);
	paintChar(char_, '-', fontSize, ref_, refFont_);
	
	QImage imageRef(imageWidth, trackHeight + 1, QImage::Format_RGB32);
	QPainter painterRef(&imageRef);
	
	for ( int i = 0; i < posEnd - posStart + 1; i++ )
	{
		int x = i * imageWidth / (posEnd - posStart + 1);
		
		const QImage * charImage = 0;
		
		switch (seq[0][i])
		{
			case 'A': charImage = &charA; break;
			case 'C': charImage = &charC; break;
			case 'G': charImage = &charG; break;
			case 'N': charImage = &charN; break;
			case 'T': charImage = &charT; break;
			case '-': charImage = &char_; break;
		}
		
		if ( charImage )
		{
			painterRef.drawImage(x, 0, *charImage);
		}
	}
	
	if ( baseWidth < 2 )
	{
		painter.setOpacity(baseWidth - 1);
	}
	
	//if ( )
	for ( int i = 0; i < getTrackCount(); i++ )
	{
		painter.drawImage(0, getTrackHeight(i) + frameWidth(), imageRef);
	}
	
	painter.setOpacity(1);
	
	unsigned int firstSnp = alignment->getNextSnpIndex(posStart);
	
	for ( int i = firstSnp; i < alignment->getSnpCount() && alignment->getSnpPosition(i) >= posStart && alignment->getSnpPosition(i) <= posEnd; i++ )
	{
		for ( int j = 0; j < alignment->getSnpCountByPosition(i); j++ )
		{
			const Alignment::Snp & snp = alignment->getSnpByPosition(i, j);
			int x = (alignment->getSnpPosition(i) - posStart) * imageBuffer->width() / (posEnd - posStart + 1);
			const QImage * charImage = 0;
			
			if ( alignment->filter(snp.filters) )
			{
				switch (snp.snp)
				{
					case 'A': charImage = &charSnpA; break;
					case 'C': charImage = &charSnpC; break;
					case 'G': charImage = &charSnpG; break;
					case 'N': charImage = &charSnpN; break;
					case 'T': charImage = &charSnpT; break;
					case '-': charImage = &charSnp_; break;
				}
				
				painter.setOpacity(1);
			}
			else
			{
				if ( baseWidth < 2 )
				{
					painter.setOpacity(baseWidth - 1);
				}
				
				switch (snp.snp)
				{
					case 'A': charImage = &charA; break;
					case 'C': charImage = &charC; break;
					case 'G': charImage = &charG; break;
					case 'N': charImage = &charN; break;
					case 'T': charImage = &charT; break;
					case '-': charImage = &char_; break;
				}
			}
			
			if ( charImage )
			{
				int track = getTrackById(snp.pos);
				int height = getTrackHeight(track + 1) - getTrackHeight(track) + 1;
				int width = (alignment->getSnpPosition(i) - posStart + 1) * imageWidth / (posEnd - posStart + 1) - x;
				painter.drawImage(QRect(x, getTrackHeight(track), width, height), *charImage, QRect(0, 0, width, height));
			}
		}
	}
}

void BlockViewMain::paintChar(QImage & image, char chr, int size, QColor color, QColor colorFont) const
{
	QPainter painter(&image);
	
	int shade;
	
	if ( size > 6 )
	{
		shade = 255;
	}
	else
	{
		shade = size * 255 / 6;
	}
	
	image.fill(color);
	painter.setPen(QColor::fromRgba(qRgba(colorFont.red(), colorFont.green(), colorFont.blue(), shade)));
	painter.setFont(QFont(painter.font().family(), size));
	painter.setBackground(color);
	painter.drawText(0, 1, image.width(), image.height() - 1, Qt::AlignCenter | Qt::AlignVCenter, QString("%1").arg(chr));
	//	painter.drawText(0, (image.height() - 2 * size / 3) / 2, image.width(), 2 * size / 3, Qt::AlignCenter, QString("%1").arg(chr));
}

