#ifndef PeripheralGenericWiFi_h
#define PeripheralGenericWiFi_h

#include "Peripheral.h"

class PeripheralGenericWiFi :
	public Peripheral
{
public:
	static const int Code_DeviceId = 43;

	Property* networkName, *networkPassword;

	PeripheralGenericWiFi(IClusterDevice* device = null);
	virtual ~PeripheralGenericWiFi();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralGenericWiFi_h
