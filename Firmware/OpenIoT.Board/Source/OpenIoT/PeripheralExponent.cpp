//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralExponent.h"

#include "Math.h"

PeripheralExponent::PeripheralExponent(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

PeripheralExponent::~PeripheralExponent()
{

}


int PeripheralExponent::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->input = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->output = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	this->exponent = *(float*)&charCode;
	charCode += 4;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralExponent::Update()
{
	this->output->SetFloat(Math::Power(this->input->fValue, this->exponent));
}
