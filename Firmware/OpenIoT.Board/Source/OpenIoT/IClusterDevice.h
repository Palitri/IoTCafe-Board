//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#ifndef IClusterDevice_h
#define IClusterDevice_h

class IPersistence;
class IWebClient;
class ISMSClient;
class Timer;
class IPropertiesObject;
class ITransmissionChannel;
class PropertyTransmissionProtocol;

class IClusterDevice
{
public:
	virtual IPersistence* GetPersistence() = 0;

	virtual void* GetUid() = 0;

	virtual void SetUpdateInterval(float seconds) = 0;

	virtual IWebClient* GetWebClient() = 0;
	virtual void SetWebClient(IWebClient* webClient) = 0;

	virtual ISMSClient* GetSMSClient() = 0;
	virtual void SetSMSClient(ISMSClient* smsClient) = 0;

	virtual ITransmissionChannel* GetTransmissionChannel() = 0;
	virtual void SetTransmissionChannel(ITransmissionChannel* transmissionChannel) = 0;

	virtual PropertyTransmissionProtocol* GetTransmissionProtocol() = 0;
		
	virtual Timer* GetTimer() = 0;

	virtual IPropertiesObject* GetPropertiesObject() = 0;

	virtual char* GetDeviceName() = 0;

	virtual void SetWifiCredentials(char *networkName,char *networkPassword) = 0;
	virtual bool GetWifiCredentials(char **networkName, char **networkPassword) = 0;

	virtual void SetBluetoothEnabled(bool enabled) = 0;
	virtual bool IsBluetoothEnabled() = 0;
};

#endif
// IClusterDevice_h
