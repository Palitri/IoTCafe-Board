#ifndef PeripheralUSB_h
#define PeripheralUSB_h

#include "Peripheral.h"

#include "ITransmissionChannel.h"

// Consider merging USB and Bluetooth peripheral into a universal "Serial transmission channel". If other devices appear later, they'd either fall into this category or another peripheral will be creates (say, "SPI transmission channel" or whatever)
class PeripheralUSB :
	public Peripheral
{
private:
	ITransmissionChannel* serial;

public:
	static const int Code_DeviceId = 41;

	PeripheralUSB(IClusterDevice* device = null);
	virtual ~PeripheralUSB();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralUSB_h
