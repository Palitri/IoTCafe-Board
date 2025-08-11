//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "ShiftRegister.h"

#include "Board.h"

ShiftRegister::ShiftRegister(int shiftRegSerialPin, int shiftRegClockPin, int shiftRegStoragePin)
{
	this->shiftRegSerialPin = shiftRegSerialPin;
	this->shiftRegClockPin = shiftRegClockPin;
	this->shiftRegStoragePin = shiftRegStoragePin;

	Board::SetPinMode(this->shiftRegSerialPin, BoardPinMode_DigitalOutput);
	Board::SetPinMode(this->shiftRegClockPin, BoardPinMode_DigitalOutput);
	Board::SetPinMode(this->shiftRegStoragePin, BoardPinMode_DigitalOutput);
}

ShiftRegister::~ShiftRegister()
{

}

void ShiftRegister::SetBits(void* bits, int bitCount)
{
	Board::DigitalWrite(this->shiftRegStoragePin, false);

	for (int i = bitCount - 1; i >=0; i--)
	{
		int byteIndex = i / 8;
		int bitIndex = i % 8;

		Board::DigitalWrite(shiftRegClockPin, false);
		Board::DigitalWrite(this->shiftRegSerialPin, (((char*)bits)[byteIndex] & (1 << bitIndex)) != 0);
		Board::DigitalWrite(shiftRegClockPin, true);
	}

	Board::DigitalWrite(this->shiftRegStoragePin, true);
}
