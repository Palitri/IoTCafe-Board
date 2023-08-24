#include "PeripheralUSB.h"

#include "Board.h"

#include "SerialTransmissionChannel.h"

PeripheralUSB::PeripheralUSB(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);

	this->serial = null;
}

PeripheralUSB::~PeripheralUSB()
{
	if (this->serial != null)
		delete this->serial;
}


int PeripheralUSB::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	//unsigned int baudRate = *(unsigned int*)charCode;
	//charCode += 2;
	unsigned int baudRate = 9600;

	unsigned char serialIndex = *charCode++;

	this->serial = new SerialTransmissionChannel(serialIndex, baudRate);

	this->device->SetTransmissionChannel(this->serial);
	
	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralUSB::Update()
{
}
