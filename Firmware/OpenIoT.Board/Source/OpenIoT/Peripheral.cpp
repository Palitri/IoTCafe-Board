//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Peripheral.h"

Peripheral::Peripheral() :
	BasePropertiesObject()
{
	this->device = null;
}

Peripheral::Peripheral(IClusterDevice* device) :
	BasePropertiesObject()
{
	this->device = device;
}

Peripheral::~Peripheral()
{
	//this->SetPropertiesCapacity(0);
}

int Peripheral::Load(const void* code)
{
	return 0;
}

void Peripheral::Update()
{
}

void Peripheral::ProcessCommand(char code, const char* data, int size)
{
}
