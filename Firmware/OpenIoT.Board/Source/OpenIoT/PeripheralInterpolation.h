#ifndef PeripheralInterpolation_h
#define PeripheralInterpolation_h

#include "Peripheral.h"

class PeripheralInterpolation :
	public Peripheral
{
public:
	static const int Code_DeviceId = 23;

	static const int Mode_Linear = 0;
	static const int Mode_Exponential = 1;
	static const int Mode_Sine = 2;

	Property* input, * output;
	unsigned char mode;
	float inMin, inDelta, outMin, outDelta, exponentialSlope;

	PeripheralInterpolation(IClusterDevice* device = null);
	virtual ~PeripheralInterpolation();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralInterpolation_h


