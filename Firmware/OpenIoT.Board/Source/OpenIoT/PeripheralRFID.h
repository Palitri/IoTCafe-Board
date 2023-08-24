#ifndef PeripheralRFID_h
#define PeripheralRFID_h

#include "Peripheral.h"

#include "Property.h"

#include "ITransmissionChannel.h"

class PeripheralRFID :
	public Peripheral
{
private:
	static const int codeSize = 14;

	static const unsigned char maxEmptyReadsCount = 5;
	static const unsigned int maxEmptyReadsTime = 200;

	int codeIndex;
	char code[codeSize];

	unsigned long timeLastRead;
	unsigned char countEmptyReads;

	ITransmissionChannel* transmission;

public:
	static const int Code_DeviceId = 18;

	Property* value, * match;
	unsigned int matchValue;

	PeripheralRFID(IClusterDevice* device = null);
	virtual ~PeripheralRFID();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralRFID_h
