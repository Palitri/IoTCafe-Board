#ifndef BrushlessMotor_h
#define BrushlessMotor_h

#include "Motor.h"

#include "ServoControl.h"

class BrushlessMotor
	: public Motor
{
public:
	ServoControl* servo;
	int motorPin, minValue, maxValue;

	BrushlessMotor(int motorPin, int minValue, int maxValue);
	virtual ~BrushlessMotor();

	virtual void SetMotorTraction(float motorTraction);
};

#endif
// BrushlessMotor_h


