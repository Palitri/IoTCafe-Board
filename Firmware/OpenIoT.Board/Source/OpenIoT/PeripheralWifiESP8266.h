#ifndef PeripheralWiFi_h
#define PeripheralWiFi_h

#include "Peripheral.h"

#include "ITransmissionChannel.h"
#include "ATCommandUtils.h"
#include "IWebClient.h"
#include "ISMSClient.h"

class PeripheralWifiESP8266 :
	public Peripheral,
	public IWebClient
{
private:
	ITransmissionChannel* serial;
	ATCommandUtils* atCommandUtils;

	bool old_on;

	static ITransmissionChannel* InitializeSerial(int serialIndex);

public:
	static const int Code_DeviceId = 30;

	//char buffer[1024];

	Property* on, *networkName, *networkPassword;

	PeripheralWifiESP8266(IClusterDevice* device = null);
	virtual ~PeripheralWifiESP8266();

	virtual int Load(const void* code);
	virtual void Update();

	virtual void SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload = null, int port = 80);
	virtual int GetResponse(char* dest, int readCount);
};

#endif
// PeripheralWiFi_h
