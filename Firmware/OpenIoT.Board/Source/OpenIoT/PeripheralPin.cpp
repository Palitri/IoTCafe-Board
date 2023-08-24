#include "PeripheralPin.h"

#include "BoardPinProperty.h"

PeripheralPin::PeripheralPin(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);
}

PeripheralPin::~PeripheralPin()
{

}


int PeripheralPin::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->AddProperty(new BoardPinProperty((void**)&charCode));

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralPin::Update()
{
	this->UpdateProperties();
}
