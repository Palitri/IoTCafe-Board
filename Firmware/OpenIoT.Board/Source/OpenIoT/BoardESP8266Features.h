#ifndef BoardESP8266Features_h
#define BoardESP8266Features_h

#include "Board.h"

#include "BoardFeatures.h"

#ifdef BOARD_ESP8266

#include "IClusterDevice.h"

#include "BoardESP8266WebClient.h"

class BoardESP8266Features :
	public BoardFeatures
{
private:
	BoardESP8266WebClient* webClient;

public:
	BoardESP8266Features(IClusterDevice* device = null);
	virtual ~BoardESP8266Features();
};

#endif

#endif
// BoardESP8266Features_h
