#include "Board.h"

#ifdef BOARD_ESP32

#include "BoardESP32Features.h"


BoardESP32Features::BoardESP32Features(IClusterDevice* device)
{
	char* wifiNetworkName, * wifiNetworkPassword;
	device->GetWifiCredentials(&wifiNetworkName, &wifiNetworkPassword);
	this->webClient = new BoardESP32WebClient(wifiNetworkName, wifiNetworkPassword, device->GetDeviceName());

	this->bluetoothChannel = new BoardESP32BluetoothTransmissionChannel(device->GetDeviceName());

	device->SetWebClient(this->webClient);
	//device->SetTransmissionChannel(this->bluetoothChannel);
}

BoardESP32Features::~BoardESP32Features()
{
	delete this->bluetoothChannel;
	delete this->webClient;
}

#endif
