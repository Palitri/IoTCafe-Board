#ifndef PeripheralSine_h
#define PeripheralSine_h

#include "Peripheral.h"

class PeripheralSine :
	public Peripheral
{
public:
	static const int Code_DeviceId = 26;

	Property* input, * output;
	float phaseStart, phaseDelta, amplitude, offset;

	PeripheralSine(IClusterDevice* device = null);
	virtual ~PeripheralSine();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralSine_h
