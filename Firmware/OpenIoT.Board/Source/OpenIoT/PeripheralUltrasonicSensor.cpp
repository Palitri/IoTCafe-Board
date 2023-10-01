//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralUltrasonicSensor.h"

PeripheralUltrasonicSensor::PeripheralUltrasonicSensor(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);
	this->sensor = new UltrasonicSensor();
}

PeripheralUltrasonicSensor::~PeripheralUltrasonicSensor()
{
	delete this->sensor;
}

int PeripheralUltrasonicSensor::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->pDistance = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	unsigned char pinTrigger = *charCode++;
	unsigned char pinEcho = *charCode++;
	float maxDistance = *(float*)charCode;
	charCode += 4;

	this->sensor->Init(pinTrigger, pinEcho, maxDistance);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralUltrasonicSensor::Update()
{
	this->sensor->Sense();

	this->pDistance->SetFloat(this->sensor->distance);
}
