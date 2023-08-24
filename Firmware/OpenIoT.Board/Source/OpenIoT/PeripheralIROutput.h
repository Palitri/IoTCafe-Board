#ifndef PeripheralIROutput_h
#define PeripheralIROutput_h

#include "Peripheral.h"

#include "Property.h"

#include "IRProtocol.h"

class PeripheralIROutput :
	public Peripheral
{
private:
	IRProtocol* irProtocol;

	int timeLastOutput;
	bool isRepeating;

public:
	static const int Code_DeviceId = 29;

	Property* active, * code, * configuration;
	unsigned char pin;

	PeripheralIROutput(IClusterDevice* device = null);
	virtual ~PeripheralIROutput();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralIROutput_h
