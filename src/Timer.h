//
//  Timer.h
//  gavqt
//
//  Created by Brian Ondov on 1/11/13.
//
//

#ifndef __gavqt__Timer__
#define __gavqt__Timer__

#include <QDateTime>
#include <math.h>

class Timer
{
public:
	
	float getProgress() const;
	void initialize(unsigned int newDuration);
	bool update();
	
private:
	
	unsigned int duration;
	float progress;
	QDateTime timeStart;
};

inline float Timer::getProgress() const
{
	return progress;
}

const int curvature = 12;
const float curveMax = 1 / (1 + exp(-curvature / 2));

#endif /* defined(__gavqt__Timer__) */
