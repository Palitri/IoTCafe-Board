//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#ifndef ISMSClient_h
#define ISMSClient_h

class ISMSClient
{
public:
	virtual void SendSMS(const char* message, const char* recepient) = 0;
};

#endif
// ISMSClient_h
