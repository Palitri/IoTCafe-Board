#ifndef PeripheralServo_h
#define PeripheralServo_h

#include "Peripheral.h"

#include "ServoControl.h"

class PeripheralServo :
	public Peripheral
{
private:
	ServoControl* servo;
	int pin;
	Property* pValue;

public:
	static const int Code_DeviceId = 14;

	PeripheralServo(IClusterDevice* device = null);
	virtual ~PeripheralServo();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralServo_h


