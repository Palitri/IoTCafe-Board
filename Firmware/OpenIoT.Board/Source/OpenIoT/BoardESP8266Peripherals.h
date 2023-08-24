#ifndef BoardESP8266Peripherals_h
#define BoardESP8266Peripherals_h

#include "Peripheral.h"

class BoardESP8266Peripherals :
	public Peripheral
{
public:
	Property* wifiNetworkName, *wifiNetworkPassword;

	BoardESP8266Peripherals(IClusterDevice* device = null);
	virtual ~BoardESP8266Peripherals();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// BoardESP8266Peripherals_h
