#ifndef PeripheralAirSensor_h
#define PeripheralAirSensor_h

#include "Peripheral.h"

#include "AirSensorBMX.h"

class PeripheralAirSensor :
	public Peripheral
{
private:
	AirSensorBMX* device;

public:
	static const int Code_DeviceId = 44;

	Property *temperature, *pressure, *humidity;

	PeripheralAirSensor(IClusterDevice* device = null);
	virtual ~PeripheralAirSensor();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralAirSensor_h
