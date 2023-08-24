#ifndef OpenIoT_h
#define OpenIoT_h

#include "Log.h"

#include "OpenIoTTransmissionProtocol.h"
#include "OpenIoTCodeProcessor.h"
#include "OpenIoTPersistence.h"

#include "Strings.h"

// Framework namespace to contain handlers for each device
// Assembly namespace to contain descendants of the Framework devices, but implementing IPropertiesObject

void OpenIoTInitialize();
void OpenIoTRun();



class OpenIoT :
	public Cluster,
	public IClusterDevice
{
private:
	OpenIoTPersistence* persistence;

	OpenIoTTransmissionProtocol* transmissionProtocol;

	OpenIoTCodeProcessor* codeProcessor;

	IWebClient* webClient  = null;
	ISMSClient* smsClient = null;

	char* wifiNetworkName, * wifiNetworkPassword;


	float updateInterval = 1.0f;

	void SendPropertyUpdates();
	void WriteEepRomProperties();

public:
	Timer* timer;
	double lastUpdateTime = 0.0;

	OpenIoT(ITransmissionChannel* transmissionChannel);

	void Run();

	virtual IPersistence* GetPersistence();

	virtual void* GetUid();

	virtual void SetUpdateInterval(float seconds);

	virtual IWebClient* GetWebClient();
	virtual void SetWebClient(IWebClient* webClient);

	virtual ISMSClient* GetSMSClient();
	virtual void SetSMSClient(ISMSClient* smsClient);

	virtual ITransmissionChannel* GetTransmissionChannel();
	virtual void SetTransmissionChannel(ITransmissionChannel* transmissionChannel);

	virtual PropertyTransmissionProtocol* GetTransmissionProtocol();

	virtual Timer* GetTimer();

	virtual IPropertiesObject* GetPropertiesObject();

	virtual char* GetDeviceName();

	virtual void GetWifiCredentials(char** networkName, char** networkPassword);
	virtual void SetWifiCredentials(char* networkName, char* networkPassword);
};

#endif
