#include "BoardESP8266Peripherals.h"

#include "Board.h"
#include "Math.h"

BoardESP8266Peripherals::BoardESP8266Peripherals(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

BoardESP8266Peripherals::~BoardESP8266Peripherals()
{

}


int BoardESP8266Peripherals::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->wifiNetworkName = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));
	this->wifiNetworkPassword = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));

	return (unsigned int)charCode - (unsigned int)code;
}

void BoardESP8266Peripherals::Update()
{

}
