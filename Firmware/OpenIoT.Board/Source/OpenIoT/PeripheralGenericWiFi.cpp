//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralGenericWiFi.h"

#include "Board.h"
#include "Math.h"

PeripheralGenericWiFi::PeripheralGenericWiFi(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

PeripheralGenericWiFi::~PeripheralGenericWiFi()
{

}


int PeripheralGenericWiFi::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->networkName = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));
	this->networkPassword = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));

	this->device->SetWifiCredentials(this->networkName->GetStringData(), this->networkPassword->GetStringData());

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralGenericWiFi::Update()
{

}
