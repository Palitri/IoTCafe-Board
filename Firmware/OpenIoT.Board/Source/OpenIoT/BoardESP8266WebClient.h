#ifndef BoardESP8266WebClient_h
#define BoardESP8266WebClient_h

#include "Board.h"

#ifdef BOARD_ESP8266

#include "IWebClient.h"

class BoardESP8266WebClient :
	public IWebClient
{
private:
	int responsePending;
	int responseReadIndex;
	char* response;

public:
	BoardESP8266WebClient(char* wifiNetworkName, char* wifiNetworkPassword, char* clientName);
	virtual ~BoardESP8266WebClient();

	virtual void SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload = null, int port = 80);
	virtual int GetResponse(char* dest, int readCount);
};

#endif

#endif
// BoardESP8266WebClient_h