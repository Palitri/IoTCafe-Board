//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "ServoControl.h"

#include "Board.h"

ServoControl::ServoControl(int pin)
{
	this->pin = pin;

	this->servo = Board::ServoBegin(pin);
}

void ServoControl::WriteMicroseconds(int microseconds)
{
	Board::ServoWrite(this->servo, microseconds);
}

void ServoControl::WriteDegreece(float degreece)
{
	//this->servo->write(degreece);
}

void ServoControl::Begin(float origin, float vector)
{

}

void ServoControl::Drive(float phase)
{

}
