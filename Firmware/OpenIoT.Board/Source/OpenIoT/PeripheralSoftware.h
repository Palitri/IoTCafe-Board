#ifndef PeripheralSoftware_h
#define PeripheralSoftware_h

#include "Peripheral.h"

class PeripheralSoftware :
	public Peripheral
{
private:

public:
	static const int Code_DeviceId = 11;

	PeripheralSoftware(IClusterDevice* device = null);
	virtual ~PeripheralSoftware();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralSoftware_h


