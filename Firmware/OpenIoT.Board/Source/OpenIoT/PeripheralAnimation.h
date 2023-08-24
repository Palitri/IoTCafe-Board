#ifndef PeripheralAnimation_h
#define PeripheralAnimation_h

#include "Peripheral.h"

class PeripheralAnimation :
	public Peripheral
{
private:
	bool back;
	unsigned long lastUpdateTime;

public:
	static const int Code_DeviceId = 25;

	Property* active, * start, * length, * speed, * output;
	bool forthAndBack, loop;

	PeripheralAnimation(IClusterDevice* device = null);
	virtual ~PeripheralAnimation();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralAnimation_h
