// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "Timer.h"

Timer::Timer()
{
	progress = 1;
}

void Timer::initialize(unsigned int newDuration)
{
	duration = newDuration;
	timeStart = QDateTime::currentDateTime();
	progress = 0;
}

bool Timer::update()
{
	QDateTime timeCurrent = QDateTime::currentDateTime();

	float progressLast = progress;

	if ( timeStart.msecsTo(timeCurrent) <= duration )
	{
		progressRaw = (float)timeStart.msecsTo(timeCurrent) / duration;
		progress =
		(1 / (1 + exp(-curvature * (progressRaw - .5))) - .5) /
		(curveMax - .5) / 2 + .5;
	}
	else
	{
		progress = 1;
	}
	
	return progress != progressLast;
}
