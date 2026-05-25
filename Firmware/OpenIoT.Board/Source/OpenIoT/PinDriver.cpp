//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PinDriver.h"

#include "Math.h"

PinDriver::PinDriver(int pin, BoardPinMode mode)
{
	this->pin = pin;
	this->pinMode = mode;	

	Board::SetPinMode(this->pin, this->pinMode);
}

PinDriver::~PinDriver()
{
}

void PinDriver::SetFloat(float value)
{
	value = Math::Trim(value, 0.0f, 1.0f);

	switch (this->pinMode)
	{
		case BoardPinMode_DigitalOutput:
			Board::DigitalWrite(this->pin, value >= 0.5f);
			break;

		case BoardPinMode_PWMOutput:
			Board::SetPwm(this->pin, value);
			break;
	}	
}

float PinDriver::GetFloat()
{
	switch (this->pinMode)
	{
		case BoardPinMode_DigitalInput:
		case BoardPinMode_DigitalInput_Pullup:
			return Board::DigitalRead(this->pin) ? 1.0f : 0.0f;

		case BoardPinMode_AnalogInput:
			return Board::AnalogRead(this->pin);
	}

	return 0.0f;
}

void PinDriver::SetBool(bool value)
{
	switch (this->pinMode)
	{
		case BoardPinMode_DigitalOutput:
			Board::DigitalWrite(this->pin, value);
			break;

		case BoardPinMode_PWMOutput:
			Board::SetPwm(this->pin, value ? 1.0f : 0.0f);
			break;
	}
}

bool PinDriver::GetBool()
{
	switch (this->pinMode)
	{
		case BoardPinMode_DigitalInput:
		case BoardPinMode_DigitalInput_Pullup:
			return Board::DigitalRead(this->pin);

		case BoardPinMode_AnalogInput:
			return Board::AnalogRead(this->pin) >= 0.5 ? true : false;
	}

	return false;
}

/// IDeviceDriver methods
void PinDriver::Begin(float origin, float vector)
{
	this->origin = origin;
	this->vector = vector;
}

void PinDriver::Drive(float phase)
{
	float value = this->origin + this->vector * phase;
	this->SetFloat(value);
}
