#ifndef PeripheralPin_h
#define PeripheralPin_h

#include "Peripheral.h"
#include "BoardPinProperty.h"

class PeripheralPin :
	public Peripheral
{
private:
	static const int CommandCode_SetPWM = 1;

	BoardPinProperty *value;

public:
	static const int Code_DeviceId = 13;

	PeripheralPin(IClusterDevice* device = null);
	virtual ~PeripheralPin();

	virtual int Load(const void* code);
	virtual void Update();

	virtual bool ProcessCommand(unsigned char command, void* data, int dataSize);
};

#endif
// PeripheralPin_h


