//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Board.h"

#ifdef BOARD_ESP8266

#include "BoardESP8266WebClient.h"

#include "Strings.h"
#include "Mem.h"
#include "Math.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "Log.h"

BoardESP8266WebClient::BoardESP8266WebClient(char* wifiNetworkName, char* wifiNetworkPassword, char* clientName)
{
	this->responsePending = false;
	this->responseReadIndex = 0;
	this->response = null;

	if (clientName != null)
		WiFi.hostname(clientName);
	
	if ((wifiNetworkName != null) && (wifiNetworkPassword != null))
		WiFi.begin(wifiNetworkName, wifiNetworkPassword);
}

BoardESP8266WebClient::~BoardESP8266WebClient()
{
	Strings::Delete(this->response);
	WiFi.disconnect(true);
}

void BoardESP8266WebClient::SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload, int port)
{
	if (WiFi.status() != WL_CONNECTED)
		return;

	char* url = Strings::CreateMerge("http://", server, ":", Strings::ToDec(port), path);

	WiFiClient client;
	HTTPClient http;

	http.begin(client, url);
	http.addHeader("Content-Type", "application/json");

	int responseCode = (payload == null) ? http.GET() : http.POST(payload);

	if (responseCode > 0)
	{
		Strings::Delete(this->response);

		this->response = Strings::CreateCopy(http.getString().c_str());
		this->responsePending = true;
		this->responseReadIndex = 0;

		Log::println(this->response);
	}

	http.end();

	Strings::Delete(url);
}

int BoardESP8266WebClient::GetResponse(char* dest, int readCount)
{
	if (this->responsePending)
	{
		int length = Strings::Length(this->response);
		int copyLength = Math::Min(length, readCount);

		Mem::Copy(dest, this->response + this->responseReadIndex, copyLength);

		this->responseReadIndex += copyLength;

		if (this->responseReadIndex >= length)
			this->responsePending = false;

		return copyLength;
	}
	else
		return 0;
}

#endif