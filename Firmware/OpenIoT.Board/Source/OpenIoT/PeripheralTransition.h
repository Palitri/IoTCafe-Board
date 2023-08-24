#ifndef PeripheralTransition_h
#define PeripheralTransition_h

#include "Peripheral.h"

class PeripheralTransition :
	public Peripheral
{
private:
	unsigned long lastUpdateTime;

public:
	static const int Code_DeviceId = 21;

	Property* input, * output;
	bool exponential;
	float speed;

	PeripheralTransition(IClusterDevice* device = null);
	virtual ~PeripheralTransition();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralTransition_h


