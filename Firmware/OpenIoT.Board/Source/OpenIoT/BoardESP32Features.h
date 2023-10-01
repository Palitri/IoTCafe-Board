#ifndef BoardESP32Features_h
#define BoardESP32Features_h

#include "Board.h"

#include "BoardFeatures.h"

#ifdef BOARD_ESP32

//#include "Peripheral.h" ?? Maybe..

#include "IClusterDevice.h"

#include "BoardESP32WebClient.h"
#include "BoardESP32BluetoothTransmissionChannel.h"

class BoardESP32Features :
	public BoardFeatures
{
private:
	static BoardESP32BluetoothTransmissionChannel* bluetoothChannel;
	static BoardESP32WebClient* webClient;

public:
	BoardESP32Features(IClusterDevice* device = null);
	virtual ~BoardESP32Features();
};

#endif

#endif
// BoardESP32Features_h
