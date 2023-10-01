#ifndef PeripheralGenericBluetooth_h
#define PeripheralGenericBluetooth_h

#include "Peripheral.h"

class PeripheralGenericBluetooth :
	public Peripheral
{
public:
	static const int Code_DeviceId = 44;

	PeripheralGenericBluetooth(IClusterDevice* device = null);
	virtual ~PeripheralGenericBluetooth();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralGenericBluetooth_h
