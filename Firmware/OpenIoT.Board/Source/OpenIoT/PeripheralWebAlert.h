#ifndef PeripheralWebAlert_h
#define PeripheralWebAlert_h

#include "Peripheral.h"

#include "ITransmissionChannel.h"

class PeripheralWebAlert :
	public Peripheral
{
private:
	bool old_send;

	static const char* notificationsEndpoint;

	virtual void SendNotification();

	char* endpoint;

public:
	static const int Code_DeviceId = 35;

	Property *send, *pushNotification, *email, *sms;

	PeripheralWebAlert(IClusterDevice* device = null);
	virtual ~PeripheralWebAlert();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralWebAlert_h
