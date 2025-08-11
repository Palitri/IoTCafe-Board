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

	virtual void ProcessCommand(char code, const char* data, int size);
};

#endif
// PeripheralPin_h


