//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "ShiftRegister.h"

#include "Board.h"

ShiftRegister::ShiftRegister(int shiftRegSerialPin, int shiftRegStoragePin, int shiftRegClockPin, int bitCount)
{
	this->shiftRegSerialPin = shiftRegSerialPin;
	this->shiftRegStoragePin = shiftRegStoragePin;
	this->shiftRegClockPin = shiftRegClockPin;
	this->bitCount = bitCount;

	Board::SetPinMode(this->shiftRegSerialPin, BoardPinMode_DigitalOutput);
	Board::SetPinMode(this->shiftRegStoragePin, BoardPinMode_DigitalOutput);
	Board::SetPinMode(this->shiftRegClockPin, BoardPinMode_DigitalOutput);

	this->bitMask = 1 << (this->bitCount - 1);

	this->SetBits(0, true);
}

ShiftRegister::~ShiftRegister()
{

}

void ShiftRegister::SetBits(int bits, bool force)
{
	if ((bits == this->bits) && (!force))
		return;

	Board::DigitalWrite(this->shiftRegStoragePin, false);

	for (int i = this->bitCount - 1; i >= 0; i--)
	{
		Board::DigitalWrite(shiftRegClockPin, false);
		Board::DigitalWrite(this->shiftRegSerialPin, (bits & this->bitMask) != 0);
		Board::DigitalWrite(shiftRegClockPin, true);

		bits <<= 1;
	}

	Board::DigitalWrite(this->shiftRegStoragePin, true);

	this->bits = bits;
}


