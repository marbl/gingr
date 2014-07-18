// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__Timer__
#define __gavqt__Timer__

#include <QDateTime>
#include <math.h>

class Timer
{
public:
	
	Timer();
	
	float getProgress() const;
	float getProgressRaw() const;
	void initialize(unsigned int newDuration);
	bool update();
	
private:
	
	unsigned int duration;
	float progress;
	float progressRaw;
	QDateTime timeStart;
};

inline float Timer::getProgress() const {return progress;}
inline float Timer::getProgressRaw() const {return progressRaw;}
const int curvature = 12;
const float curveMax = 1 / (1 + exp(-curvature / 2));

#endif /* defined(__gavqt__Timer__) */
