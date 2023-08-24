#ifndef BoardESP32WebClient_h
#define BoardESP32WebClient_h

#include "Board.h"

#ifdef BOARD_ESP32

#include "IWebClient.h"

class BoardESP32WebClient :
	public IWebClient
{
private:
	int responsePending;
	int responseReadIndex;
	char* response;

public:
	BoardESP32WebClient(char* wifiNetworkName, char* wifiNetworkPassword, char* clientName);
	virtual ~BoardESP32WebClient();

	virtual void SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload = null, int port = 80);
	virtual int GetResponse(char* dest, int readCount);
};

#endif

#endif
// BoardESP32WebClient