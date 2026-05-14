// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

// ColumnConservationView.cpp
// Plots per-window column conservation (Average Nucleotide Identity) of all
// non-reference genomes relative to the reference.  The reference is always
// 100 %.  Windows with no SNP columns show 100 % (perfectly conserved).

#include "ColumnConservationView.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>

ColumnConservationView::ColumnConservationView()
    : alignment(nullptr)
    , posStart(0)
    , posEnd(0)
{
    setMinimumHeight(40);
    setMaximumHeight(40);
}

void ColumnConservationView::setAlignment(Alignment * alignmentNew)
{
    alignment = alignmentNew;
}

void ColumnConservationView::setWindow(int start, int end)
{
    posStart = start;
    posEnd   = end;
    update();
}

void ColumnConservationView::clear()
{
    alignment = nullptr;
    update();
}

// ---------------------------------------------------------------------------
//  calculateConservation
//
//  Slides a window across [posStart, posEnd].  For each window:
//    - scan every SnpColumn whose position falls in [winStart, winEnd]
//    - each such column has `ref` and a list of per-genome SNPs
//    - for every non-reference genome, a SNP entry means that genome
//      differs from the reference at that column
//    - Conservation % = (matched bases / total bases) * 100
//      where total bases = numNonRefTracks * numSnpColumnsInWindow
//      and  matched bases = total - mismatch count
//
//  Windows with no SNP columns are perfectly conserved (100 %).
// ---------------------------------------------------------------------------
std::vector<ColumnConservationView::ConservationPoint> ColumnConservationView::calculateConservation() const
{
    std::vector<ConservationPoint> results;
    if (!alignment) return results;

    const int totalLen     = posEnd - posStart + 1;
    const int targetPoints = width() / 3;
    int windowSize         = std::max(1, totalLen / targetPoints);
    const int stepSize     = std::max(1, windowSize / 2);

    const int refTrack     = alignment->getTrackReference();
    const int trackTotal   = alignment->getTracks() ? (int)alignment->getTracks()->size() : 0;
    const int nonRefTracks = std::max(0, trackTotal - 1);

    if (nonRefTracks == 0) return results;

    const int snpCount = alignment->getSnpColumnCount();

    // Advance to first SNP column >= posStart.
    int firstSnpIdx = 0;
    while (firstSnpIdx < snpCount &&
           alignment->getSnpColumn(firstSnpIdx).position < posStart)
    {
        firstSnpIdx++;
    }

    for (int winStart = posStart; winStart <= posEnd; winStart += stepSize)
    {
        int winEnd = winStart + windowSize - 1;
        if (winEnd > posEnd) winEnd = posEnd;

        ConservationPoint point;
        point.position = (winStart + winEnd) / 2;
        point.valid    = false;
        point.ani      = 100.0;

        long long mismatches = 0;
        long long columns    = 0;

        for (int si = firstSnpIdx; si < snpCount; si++)
        {
            const Alignment::SnpColumn & col = alignment->getSnpColumn(si);
            if (col.position < winStart) { firstSnpIdx = si + 1; continue; }
            if (col.position > winEnd)   break;

            for (const Alignment::Snp & snp : col.snps)
            {
                if (snp.track != refTrack)
                    mismatches++;
            }
            columns++;
        }

        if (columns == 0)
        {
            point.ani   = 100.0;
            point.valid = true;
        }
        else
        {
            long long totalBases = (long long)nonRefTracks * columns;
            double matched = (double)(totalBases - mismatches);
            point.ani   = 100.0 * matched / (double)totalBases;
            point.valid = true;
        }

        results.push_back(point);
    }

    return results;
}

void ColumnConservationView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    if (!alignment) return;

    auto data = calculateConservation();
    if (data.empty()) return;

    const int    margin = 4;
    const int    drawH  = height() - 2 * margin;
    const double span   = posEnd - posStart + 1;

    // Y: 100 % at top, 0 % at bottom.
    auto toY = [&](double pct) -> int {
        double clamped = std::max(0.0, std::min(100.0, pct));
        return height() - margin - (int)(clamped / 100.0 * drawH);
    };

    auto toX = [&](int pos) -> int {
        return (int)((double)(pos - posStart) / span * width());
    };

    // Fill from top down to the line (highlights dips/lost conservation)
    // Fill from top down to the line (highlights dips/lost conservation)
    {
        QPainterPath area;
        bool started = false;
        int lastX = 0;

        for (const auto & pt : data)
        {
            if (!pt.valid) { started = false; continue; }
            int x = toX(pt.position);
            int y = toY(pt.ani);
            if (!started) {
                area.moveTo(x, margin);
                area.lineTo(x, y);
                started = true;
            } else {
                area.lineTo(x, y);
            }
            lastX = x;
        }
        if (started) {
            area.lineTo(lastX, margin);
            area.closeSubpath();
        }

        painter.setBrush(QColor(220, 50, 50, 80));  
        painter.setPen(Qt::NoPen);
        painter.drawPath(area);
    }

    // --- line on top ---
    {
        QPainterPath line;
        bool started = false;
        for (const auto & pt : data)
        {
            if (!pt.valid) { started = false; continue; }
            int x = toX(pt.position);
            int y = toY(pt.ani);
            if (!started) { line.moveTo(x, y); started = true; }
            else            line.lineTo(x, y);
        }
        painter.setPen(QPen(QColor(180, 30, 30), 1));
        painter.drawPath(line);
    }

    // --- 100 % baseline (dashed) ---
    painter.setPen(QPen(QColor(180, 180, 180), 1, Qt::DashLine));
    painter.drawLine(0, toY(100.0), width(), toY(100.0));

    // --- label ---
    painter.setPen(Qt::black);
    QFont f = painter.font();
    f.setPointSize(7);
    painter.setFont(f);
    painter.drawText(4, margin + 9, "Conservation %");

    // --- right-side scale ticks ---
    painter.setPen(Qt::black);
    for (int pct : {0, 50, 100})
    {
        int y = toY(pct);
        painter.drawLine(width() - 18, y, width(), y);
        painter.drawText(width() - 36, y + 4, QString::number(pct));
    }
    // --- 100 % baseline (dashed) ---
    painter.setPen(QPen(QColor(180, 180, 180), 1, Qt::DashLine));
    painter.drawLine(0, toY(100.0), width(), toY(100.0));

    // --- label ---
    painter.setPen(Qt::black);
    f = painter.font();
    f.setPointSize(7);
    painter.setFont(f);
    painter.drawText(4, margin + 9, "Conservation %");

    // --- right-side scale ticks ---
    painter.setPen(QColor(120, 120, 120));
    for (int pct : {0, 50, 100})
    {
        int y = toY(pct);
        painter.drawLine(width() - 18, y, width(), y);
        painter.drawText(width() - 36, y + 4, QString::number(pct));
    }
}