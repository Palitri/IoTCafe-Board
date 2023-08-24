#ifndef PeripheralExponent_h
#define PeripheralExponent_h

#include "Peripheral.h"

class PeripheralExponent :
	public Peripheral
{
public:
	static const int Code_DeviceId = 27;

	Property* input, * output;
	float exponent;

	PeripheralExponent(IClusterDevice* device = null);
	virtual ~PeripheralExponent();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralExponent_h
