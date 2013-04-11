//
//  Tween.h
//  gavqt
//
//  Created by Brian Ondov on 8/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef gavqt_Tween_h
#define gavqt_Tween_h

class Tween
{
public:
	
	Tween();
	
	void initialize(float newStart, float newEnd);
	void setTarget(float target, bool finish = false);
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
inline float Tween::getEnd() const {return end;}
#endif
