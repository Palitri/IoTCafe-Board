#ifndef PropertyTransmissionProtocol_h
#define PropertyTransmissionProtocol_h

#include "CommandTransmissionProtocol.h"
#include "IPropertiesObject.h"

class PropertyTransmissionProtocol :
	public CommandTransmissionProtocol
{
private:
	void SubscriptionRoutines();
	int GetPropertyFeedCommandSize(Property* property);
	void FeedCommandWithPropertyValue(Property* property);

	unsigned long timeLastResponse;
	unsigned long timeLastPingRequest;
	bool clientSubscribed;

protected:
	//static const unsigned char ResponseCodeXORMap = 0xFF;

	static const unsigned char Code_ResponseBit = 0x80;

	static const unsigned char CommandCode_Ping						= 0x10;
	static const unsigned char CommandCode_Info						= 0x11;
	static const unsigned char CommandCode_QueryParamsInfo			= 0x21;
	static const unsigned char CommandCode_QueryParamsValues		= 0x22;
	static const unsigned char CommandCode_SetParamsValues			= 0x23;
	static const unsigned char CommandCode_SetParamsSubscription	= 0x24;
	static const unsigned char CommandCode_ResetParamsSubscription	= 0x25;

	static const unsigned char ResponseCode_Ping					= 0x10 | Code_ResponseBit;
	static const unsigned char ResponseCode_Info					= 0x11 | Code_ResponseBit;
	static const unsigned char ResponseCode_QueryParamsInfo			= 0x21 | Code_ResponseBit;
	static const unsigned char ResponseCode_QueryParamsValues		= 0x22 | Code_ResponseBit;
	static const unsigned char ResponseCode_SetParamsValues			= 0x23 | Code_ResponseBit;
	static const unsigned char ResponseCode_SetParamsSubscription	= 0x24 | Code_ResponseBit;
	static const unsigned char ResponseCode_ResetParamsSubscription	= 0x25 | Code_ResponseBit;

	static const unsigned char ResponseCode_SubscribedParamsChanged	= 0x30 | Code_ResponseBit;


	static const unsigned int Timeout_TransmissionIdle_Milliseconds = 5000;
	static const unsigned int Timeout_StartPingAfterIdle_Milliseconds = 2500;
	static const unsigned int Timeout_PingInterval_Milliseconds = 1000;

protected:
	IPropertiesObject* propertiesObject;

public:
	PropertyTransmissionProtocol(IPropertiesObject* propertiesObject);
	virtual ~PropertyTransmissionProtocol();

	virtual void ProcessInput() override;

	virtual void SetPropertiesObject(IPropertiesObject* propertiesObject);

	virtual void OnReceiveChunk(void* data, int dataSize);

	virtual bool OnReceiveCommand(unsigned char command, void* data, int dataSize);

	virtual void SendChangedSubscribedProperties();

	virtual void ResetPropertiesSubscription();

	void RequestPing();
};

#endif
// PropertyTransmissionProtocol_h
