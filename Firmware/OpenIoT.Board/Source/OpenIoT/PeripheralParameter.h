#ifndef PeripheralParameter_h
#define PeripheralParameter_h

#include "Peripheral.h"

class PeripheralParameter :
	public Peripheral
{
public:
	static const int Code_DeviceId = 20;

	Property* parameter;

	PeripheralParameter(IClusterDevice* device = null);
	virtual ~PeripheralParameter();

	virtual int Load(const void* code);
};

#endif
// PeripheralParameter_h


