//
//  Tween.cpp
//  gavqt
//
//  Created by Brian Ondov on 8/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Tween.h"

Tween::Tween()
{
	start = 0;
	end = 0;
	value = 0;
	factor = 0;
}

void Tween::initialize(float newStart, float newEnd)
{
	start = newStart;
	end = newEnd;
	value = newEnd;
	factor = end - start;
}

void Tween::setTarget(float target, bool finish)
{
	start = finish ? end : value;
	end = target;
	factor = end - start;
}

void Tween::update(float progress)
{
	if ( progress == 0 )
	{
		value = start;
	}
	else if ( progress == 1 )
	{
		value = end;
	}
	else
	{
		value = start + progress * factor;
	}
}
