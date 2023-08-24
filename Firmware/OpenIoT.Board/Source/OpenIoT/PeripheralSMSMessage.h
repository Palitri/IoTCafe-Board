#ifndef PeripheralSMSMessage_h
#define PeripheralSMSMessage_h

#include "Peripheral.h"

class PeripheralSMSMessage :
	public Peripheral
{
private:
	bool old_send;

public:
	static const int Code_DeviceId = 40;

	Property* send, *code;

	PeripheralSMSMessage(IClusterDevice* device = null);
	virtual ~PeripheralSMSMessage();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralSMSMessage_h
