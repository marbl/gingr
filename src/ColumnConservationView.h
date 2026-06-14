// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#pragma once
#include <QWidget>
#include <vector>
#include "Alignment.h"

class ColumnConservationView : public QWidget
{
    Q_OBJECT

public:
    ColumnConservationView();

    void setAlignment(Alignment * alignmentNew);
    void setWindow(int start, int end);
    void clear();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    struct ConservationPoint {
        int    position;
        double ani;   // 0.0 – 100.0
        bool   valid;
    };

    std::vector<ConservationPoint> calculateConservation() const;

    Alignment * alignment;
    int posStart;
    int posEnd;
};