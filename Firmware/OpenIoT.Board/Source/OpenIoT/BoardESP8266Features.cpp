#include "Board.h"

#ifdef BOARD_ESP8266

#include "BoardESP8266Features.h"

BoardESP8266Features::BoardESP8266Features(IClusterDevice* device)
{
	char* wifiNetworkName, * wifiNetworkPassword;
	device->GetWifiCredentials(&wifiNetworkName, &wifiNetworkPassword);
	this->webClient = new BoardESP8266WebClient(wifiNetworkName, wifiNetworkPassword, device->GetDeviceName());

	device->SetWebClient(this->webClient);
}

BoardESP8266Features::~BoardESP8266Features()
{
	delete this->webClient;
}

#endif
