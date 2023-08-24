#ifndef PeripheralWebLink_h
#define PeripheralWebLink_h

#include "Peripheral.h"
#include "StreamingInputDetector.h"

#include "ITransmissionChannel.h"

class PeripheralWebLink :
	public Peripheral
{
private:
	static const unsigned int weblinkRequestIntervalMillis;
	static const unsigned int weblinkMaxResponseTimeMillis;

	static const char* webLinkBaseUrl;
	static const char* webLinkRelaySignature;

	void SendWeblinkRequest(bool relay, bool feed);
	void ProcessRelayResponseInput();
		
	StreamingInputDetector* responseRelayParser;

	unsigned char responseState;
	bool isAwaitingResponse;
	unsigned long lastRequestTime, lastFeedTime;

	int hexIndex;
	unsigned char hexByte;

	char* GetRequestEndpoint();
	char* GetRequestPayload(bool includeDeviceName, bool requestRelay, bool includeFeed);

	void SetAwaitingResponseState(bool isAwaiting);

public:
	static const int Code_DeviceId = 34;

	Property *webFeed, *feedInterval, *webRelay;

	PeripheralWebLink(IClusterDevice* device = null);
	virtual ~PeripheralWebLink();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralWebLink_h
