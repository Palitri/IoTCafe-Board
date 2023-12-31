//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#ifndef IWebClient_h
#define IWebClient_h

#include "Board.h"

// Have a WebClient, which can connect on tcp/ip level and an HttpClient, which can do http requests?

class IWebClient
{
public:
	virtual void SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload = null, int port = 80) = 0;
	virtual int GetResponse(char* dest, int readCount) = 0;
};

#endif
// IWebClient_h
