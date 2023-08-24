#ifndef PeripheralPID_h
#define PeripheralPID_h

#include "Peripheral.h"

class PeripheralPID :
	public Peripheral
{
private:
	float errorLast, errorIntegral;
	float lastUpdateTime;

	float Compute(float error, float time);

public:
	static const int Code_DeviceId = 28;

	Property* input, * output, * target, * p, * i, * d;

	PeripheralPID(IClusterDevice* device = null);
	virtual ~PeripheralPID();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralPID_h
