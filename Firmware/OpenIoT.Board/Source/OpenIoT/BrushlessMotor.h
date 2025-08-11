#ifndef BrushlessMotor_h
#define BrushlessMotor_h

#include "Motor.h"
#include "IDeviceDriver.h"

#include "ServoControl.h"

class BrushlessMotor :
	public Motor,
	public IDeviceDriver
{
private:
	float asyncOrigin;
	float asyncVector;

public:
	ServoControl* servo;
	int motorPin, minValue, maxValue;

	BrushlessMotor(int motorPin, int minValue, int maxValue);
	virtual ~BrushlessMotor();

	virtual void SetMotorTraction(float motorTraction);

	// IDeviceDriver
	void Begin(float origin, float vector);
	void Drive(float phase);
};

#endif
// BrushlessMotor_h


