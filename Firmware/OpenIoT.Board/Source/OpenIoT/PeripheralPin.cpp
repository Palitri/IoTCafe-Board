//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralPin.h"

unsigned char PeripheralPin::pinModeToPropertyTypeMap[6] =
{
  (unsigned char)0,                    // None
  (unsigned char)PropertyType_Bool,    // BoardPinMode_DigitalInput
  (unsigned char)PropertyType_Bool,    // BoardPinMode_DigitalOutput
  (unsigned char)PropertyType_Float,   // BoardPinMode_PWMOutput
  (unsigned char)PropertyType_Float,   // BoardPinMode_AnalogInput
  (unsigned char)PropertyType_Bool     // BoardPinMode_DigitalInput_Pullup
};

unsigned char PeripheralPin::pinModeToPropertyFlagsMap[6] =
{
  (unsigned char)0,                                         // None
  (unsigned char)PropertyFlag_Read,                         // BoardPinMode_DigitalInput
  (unsigned char)PropertyFlag_Read | PropertyFlag_Write,    // BoardPinMode_DigitalOutput
  (unsigned char)PropertyFlag_Read | PropertyFlag_Write,    // BoardPinMode_PWMOutput
  (unsigned char)PropertyFlag_Read,                         // BoardPinMode_AnalogInput
  (unsigned char)PropertyFlag_Read                          // BoardPinMode_DigitalInput_Pullup
};

PeripheralPin::PeripheralPin(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);

	this->pinDriver = null;
}

PeripheralPin::~PeripheralPin()
{

}


int PeripheralPin::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	unsigned char pin = *charCode;
	charCode++;

	BoardPinMode pinMode = (BoardPinMode)*charCode;
	charCode++;

	if (this->pinDriver != null)
		delete this->pinDriver;
	this->pinDriver = new PinDriver(pin, pinMode);
	this->driver = this->pinDriver;

	PropertyType type = (PropertyType)PeripheralPin::pinModeToPropertyTypeMap[pinMode];
	unsigned char flags = PeripheralPin::pinModeToPropertyFlagsMap[pinMode];
	this->value = this->AddProperty(new Property((void**)&charCode, type, flags));

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralPin::Update()
{
	switch (this->pinDriver->pinMode)
	{
		case BoardPinMode_DigitalInput:
			this->value->SetBool(this->pinDriver->GetBool());
			break;

		case BoardPinMode_AnalogInput:
			this->value->SetFloat(this->pinDriver->GetFloat());
			break;

		case BoardPinMode_DigitalOutput:
			this->pinDriver->SetBool(this->value->bValue);
			break;

		case BoardPinMode_PWMOutput:
			this->pinDriver->SetFloat(this->value->fValue);
			break;
	}
}

bool PeripheralPin::ProcessCommand(unsigned char command, void* data, int dataSize)
{
	switch (command)
	{
		case PeripheralPin::CommandCode_SetPWM:
		{
			this->pinDriver->SetFloat(*(float*)data);
			this->value->SetFloat(*(float*)data);
			break;
		}

		default:
		{
			return Peripheral::ProcessCommand(command, data, dataSize);
		}
	}

	return true;
}
