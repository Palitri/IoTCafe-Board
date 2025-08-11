#ifndef PeripheralDigitalEncoder_h
#define PeripheralDigitalEncoder_h

#include "Peripheral.h"

class PeripheralRotaryEncoder :
	public Peripheral
{
private:
	static void InterruptHandler();

	void OnInterrupt();

	char ReadState();

	static const signed char statesMap[16];
	
	static PeripheralRotaryEncoder *interruptInstance;

	Property* count, * delta;

	char state;
	int interruptDelta;

	char pinA, pinB;

public:
	static const int Code_DeviceId = 12;

	PeripheralRotaryEncoder(IClusterDevice* device = null);
	virtual ~PeripheralRotaryEncoder();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralDigitalEncoder_h


