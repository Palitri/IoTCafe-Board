#ifndef PeripheralGSM_h
#define PeripheralGSM_h

#include "Peripheral.h"

#include "ITransmissionChannel.h"
#include "ATCommandUtils.h"
#include "IWebClient.h"
#include "ISMSClient.h"

#include "GsmSim800L.h"

class PeripheralGSM :
	public Peripheral,
	public IWebClient,
	public ISMSClient
{
private:
	ITransmissionChannel* serial;
	ATCommandUtils* atCommandUtils;

	bool old_on;

	GsmSim800L* gsmDevice;

public:
	static const int Code_DeviceId = 39;

	Property *on, *gprs, *pin, *apn, *apnUser, *apnPassword;

	PeripheralGSM(IClusterDevice* device = null);
	virtual ~PeripheralGSM();

	virtual int Load(const void* code);
	virtual void Update();

	virtual void SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload = null, int port = 80);
	virtual int GetResponse(char* dest, int readCount);

	virtual void SendSMS(const char* message, const char* recepient);
};

#endif
// PeripheralGSM_h
