#ifndef PeripheralUltrasonicSensor_h
#define PeripheralUltrasonicSensor_h

#include "Peripheral.h"
#include "UltrasonicSensor.h"

class PeripheralUltrasonicSensor :
	public Peripheral
{
private:
	Property* pDistance;
	UltrasonicSensor* sensor;

public:
	static const int Code_DeviceId = 16;

	PeripheralUltrasonicSensor(IClusterDevice* device = null);
	virtual ~PeripheralUltrasonicSensor();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralServo_h


