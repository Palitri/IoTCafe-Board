//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "BoardPinProperty.h"

unsigned char BoardPinProperty::pinModeToPropertyTypeMap[5] =
{
  (unsigned char)0,                    // None
  (unsigned char)PropertyType_Bool,    // BoardPinMode_DigitalInput
  (unsigned char)PropertyType_Bool,    // BoardPinMode_DigitalOutput
  (unsigned char)PropertyType_Float,   // BoardPinMode_PWMOutput
  (unsigned char)PropertyType_Float    // BoardPinMode_AnalogInput
};

unsigned char BoardPinProperty::pinModeToPropertyFlagsMap[5] =
{
  (unsigned char)0,                                         // None
  (unsigned char)PropertyFlag_Read,                         // BoardPinMode_DigitalInput
  (unsigned char)PropertyFlag_Read | PropertyFlag_Write,    // BoardPinMode_DigitalOutput
  (unsigned char)PropertyFlag_Read | PropertyFlag_Write,    // BoardPinMode_PWMOutput
  (unsigned char)PropertyFlag_Read                          // BoardPinMode_AnalogInput
};

BoardPinProperty::BoardPinProperty(void** data)
{
	unsigned char* charData = (unsigned char*)*data;

	this->pin = *charData;
	charData++;

	this->pinMode = (BoardPinMode)*charData;	
	charData++;

	PropertyType type = (PropertyType)BoardPinProperty::pinModeToPropertyTypeMap[this->pinMode];
	unsigned char flags = BoardPinProperty::pinModeToPropertyFlagsMap[this->pinMode];

	this->InitWithValue((void**)&charData, type, flags);

	Board::SetPinMode(this->pin, this->pinMode);

	*data = charData;

	this->Update();
}

BoardPinProperty::BoardPinProperty(int semantic, int pin, BoardPinMode mode)
{
	this->pin = pin;
	this->pinMode = mode;

	Board::SetPinMode(this->pin, this->pinMode);

	switch (this->pinMode)
	{
	case BoardPinMode_DigitalInput:
		this->Init(semantic, PropertyType_Bool, PropertyFlag_Read);
		break;

	case BoardPinMode_DigitalOutput:
		this->Init(semantic, PropertyType_Bool, PropertyFlag_Read | PropertyFlag_Write);
		break;

	case BoardPinMode_AnalogInput:
		this->Init(semantic, PropertyType_Float, PropertyFlag_Read);
		break;

	case BoardPinMode_PWMOutput:
		this->Init(semantic, PropertyType_Float, PropertyFlag_Read | PropertyFlag_Write);
		break;
	}

	this->Update();
}

void BoardPinProperty::Update()
{
	switch (this->pinMode)
	{
	case BoardPinMode_DigitalInput:
		this->SetBool(Board::DigitalRead(this->pin));
		break;

	case BoardPinMode_DigitalOutput:
		//if (this->IsChanged())
		Board::DigitalWrite(this->pin, this->bValue);
		break;

	case BoardPinMode_AnalogInput:
		this->SetFloat(Board::AnalogRead(this->pin));
		break;

	case BoardPinMode_PWMOutput:
		//if (this->IsChanged())
		Board::SetPwm(this->pin, this->fValue);
		break;
	}
}
