#ifndef PeripheralPin_h
#define PeripheralPin_h

#include "Peripheral.h"

class PeripheralPin :
	public Peripheral
{
private:

public:
	static const int Code_DeviceId = 13;

	PeripheralPin(IClusterDevice* device = null);
	virtual ~PeripheralPin();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralPin_h


