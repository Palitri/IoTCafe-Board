//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralBrushlessMotor.h"

#include "Board.h"

PeripheralBrushlessMotor::PeripheralBrushlessMotor(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);
	this->motor = null;
}

PeripheralBrushlessMotor::~PeripheralBrushlessMotor()
{
	if (this->motor != null)
		delete this->motor;
}

int PeripheralBrushlessMotor::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->pValue = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));

	unsigned char pin = *charCode;
	charCode++;
	unsigned short minValue = *(unsigned short*)charCode;
	charCode += 2;
	unsigned short maxValue = *(unsigned short*)charCode;
	charCode += 2;

	this->motor = new BrushlessMotor(pin, minValue, maxValue);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralBrushlessMotor::Update()
{
	if (this->motor == null)
		return;

	this->motor->SetTraction(this->pValue->fValue);
}
