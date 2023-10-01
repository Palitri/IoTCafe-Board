//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralParameter.h"

PeripheralParameter::PeripheralParameter(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);
}

PeripheralParameter::~PeripheralParameter()
{

}


int PeripheralParameter::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->parameter = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));

	return (unsigned int)charCode - (unsigned int)code;
}
