//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Board.h"

#ifdef BOARD_ESP32

#include "BoardESP32Features.h"


BoardESP32BluetoothTransmissionChannel* BoardESP32Features::bluetoothChannel = null;
BoardESP32WebClient* BoardESP32Features::webClient = null;

BoardESP32Features::BoardESP32Features(IClusterDevice* device)
{
	char* wifiNetworkName, * wifiNetworkPassword;
	if (device->GetWifiCredentials(&wifiNetworkName, &wifiNetworkPassword))
	{
		if (BoardESP32Features::webClient == null)
		{
			BoardESP32Features::webClient = new BoardESP32WebClient(wifiNetworkName, wifiNetworkPassword, device->GetDeviceName());
		}

		device->SetWebClient(BoardESP32Features::webClient);
	}

	if (device->IsBluetoothEnabled())
	{
		if (BoardESP32Features::bluetoothChannel == null)
		{
			BoardESP32Features::bluetoothChannel = new BoardESP32BluetoothTransmissionChannel(device->GetDeviceName());
		}

		device->SetTransmissionChannel(BoardESP32Features::bluetoothChannel);
	}
}

BoardESP32Features::~BoardESP32Features()
{
}

#endif
