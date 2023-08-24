#ifndef PeripheralDeviceDetails_h
#define PeripheralDeviceDetails_h

#include "Peripheral.h"

class PeripheralDeviceDetails :
	public Peripheral
{
public:
	static const int Code_DeviceId = 38;

	Property *sleepInterval, *updateInterval, *fps;

	PeripheralDeviceDetails(IClusterDevice* device = null);
	virtual ~PeripheralDeviceDetails();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralDeviceDetails