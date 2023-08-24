#ifndef PeripheralGSM_h
#define PeripheralGSM_h

#include "Peripheral.h"

#include "ITransmissionChannel.h"
#include "ATCommandUtils.h"
#include "IWebClient.h"
#include "ISMSClient.h"

class PeripheralGPRS :
	public Peripheral,
	public IWebClient,
	public ISMSClient
{
private:
	ITransmissionChannel* serial;
	ATCommandUtils* atCommandUtils;

	bool old_on;

public:
	static const int Code_DeviceId = 39;

	Property* on;

	PeripheralGPRS(IClusterDevice* device = null);
	virtual ~PeripheralGPRS();

	virtual int Load(const void* code);
	virtual void Update();

	virtual void SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload = null, int port = 80);
	virtual int GetResponse(char* dest, int readCount);

	virtual void SendSMS(const char* message, const char* recepient);
};

#endif
// PeripheralGSM_h
