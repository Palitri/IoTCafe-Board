//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralGenericBluetooth.h"

#include "Board.h"
#include "Math.h"

PeripheralGenericBluetooth::PeripheralGenericBluetooth(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);
}

PeripheralGenericBluetooth::~PeripheralGenericBluetooth()
{

}


int PeripheralGenericBluetooth::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->device->SetBluetoothEnabled(true);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralGenericBluetooth::Update()
{

}
