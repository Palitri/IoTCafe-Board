//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralBluetooth.h"

#include "Board.h"

#include "SerialTransmissionChannel.h"

PeripheralBluetooth::PeripheralBluetooth(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);

	this->serial = null;
}

PeripheralBluetooth::~PeripheralBluetooth()
{
	if (this->serial != null)
		delete this->serial;
}


int PeripheralBluetooth::Load(const void* code)
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

void PeripheralBluetooth::Update()
{
}
