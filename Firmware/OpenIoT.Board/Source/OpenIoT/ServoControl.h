#ifndef ServoControl_h
#define ServoControl_h

#include "IDeviceDriver.h"

class ServoControl :
	public IDeviceDriver
{
private:
	void* servo;

public:
	int pin;

	ServoControl(int pin);

	void WriteMicroseconds(int microseconds);
	void WriteDegreece(float degreece);

	// IDeviceDriver
	void Begin(float origin, float vector);
	void Drive(float phase);
};

#endif
// ServoControl_h