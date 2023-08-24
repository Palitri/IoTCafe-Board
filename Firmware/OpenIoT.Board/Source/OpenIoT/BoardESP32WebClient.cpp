#include "Board.h"

#ifdef BOARD_ESP32

#include "BoardESP32WebClient.h"

#include "Strings.h"
#include "Mem.h"
#include "Math.h"

#include <WiFi.h>
#include <HTTPClient.h>

#include "Log.h"

BoardESP32WebClient::BoardESP32WebClient(char *wifiNetworkName, char *wifiNetworkPassword, char* clientName)
{
	this->responsePending = false;
	this->responseReadIndex = 0;
	this->response = null;

	Log::print("Wifi: ");
	Log::print(wifiNetworkName != null ? wifiNetworkName : "");
	Log::print(", ");
	Log::println(wifiNetworkPassword != null ? wifiNetworkPassword : "");

	WiFi.mode(WIFI_STA);	// WIFI_STA: Station mode - connects to access point. WIFI_AP : Acceess point mode - stations can connect to it. WIFI_AP_STA: Both

	if (clientName != null)
		WiFi.setHostname(clientName);

	if ((wifiNetworkName != null) && (wifiNetworkPassword != null))
		WiFi.begin(wifiNetworkName, wifiNetworkPassword);

	//char* mac = WiFi.macAddress();
	// char* ip = WiFi.localIP()
	
	//Log::print("Wifi IP: ");
	//Log::println(WiFi.localIP());

	Log::println("Wifi started ");
}

BoardESP32WebClient::~BoardESP32WebClient()
{
	Log::print("Wifi disconnecting... ");
	Strings::Delete(this->response);
	WiFi.disconnect(true);
	Log::println("done.");
}

void BoardESP32WebClient::SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload, int port)
{
	if (WiFi.status() != WL_CONNECTED)
		return;

	char* url = Strings::CreateMerge("http://", server, ":", Strings::ToDec(port), path);

	HTTPClient http;

	http.begin(url);
	http.addHeader("Content-Type", "application/json");

	int responseCode = (payload == null) ? http.GET() : http.POST(payload);

	//Log::println(url);
	//Log::println(responseCode);
	if (responseCode > 0)
	{
		Strings::Delete(this->response);

		this->response = Strings::CreateCopy(http.getString().c_str());
		this->responsePending = true;
		this->responseReadIndex = 0;

		//Log::println(this->response);
	}

	http.end();

	Strings::Delete(url);
}

int BoardESP32WebClient::GetResponse(char* dest, int readCount)
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