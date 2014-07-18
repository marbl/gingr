// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef gavqt_Tween_h
#define gavqt_Tween_h

class Tween
{
public:
	
	Tween();
	
	void initialize(float newStart, float newEnd);
	void setTarget(float target, bool finish = false);
	float getStart() const;
	float getEnd() const;
	float getValue() const;
	void update(float progress);
	
private:
	
	float end;
	float factor;
	float start;
	float value;
};

inline float Tween::getValue() const {return value;}
inline float Tween::getStart() const {return start;}
inline float Tween::getEnd() const {return end;}
#endif
