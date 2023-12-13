//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralRotaryEncoder.h"

#include "Board.h"

PeripheralRotaryEncoder* PeripheralRotaryEncoder::interruptInstance;


// A: 000001111110000001111111
// B: 000000111111000000111111
// statesMap[delta] = oldABit, oldBBit, newABit, newBBit
const signed char PeripheralRotaryEncoder::statesMap[16] = 
{
	0,	// 0000 Same 0
	-1,	// 0001 Left 1
	1,	// 0010 Right 2
	0,	// 0011 Invalid 3
	1,	// 0100 Right 4
	0,	// 0101 Same 5
	0,	// 0110 Invalid 6
	-1,	// 0111 Left 7
	-1,	// 1000 Left 8
	0,	// 1001 Invalid 9
	0,	// 1010 Same 10
	1,	// 1011 Right 11
	0,	// 1100 Invalid 12
	1,	// 1101 Right 13
	-1,	// 1110 Left 14
	0,	// 1111 Same 15
};


PeripheralRotaryEncoder::PeripheralRotaryEncoder(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);

	this->interruptDelta = 0;
}

PeripheralRotaryEncoder::~PeripheralRotaryEncoder()
{
}


int PeripheralRotaryEncoder::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->count = this->AddProperty(new Property((void**)&charCode, PropertyType_Integer));
	this->delta = this->AddProperty(new Property((void**)&charCode, PropertyType_Integer));


	this->pinA = *charCode++;
	this->pinB = *charCode++;

	this->state = this->ReadState();

	Board::SetPinMode(this->pinA, BoardPinMode_DigitalInput_Pullup);
	Board::SetPinMode(this->pinB, BoardPinMode_DigitalInput_Pullup);

	PeripheralRotaryEncoder::interruptInstance = this;
	Board::AttachPinInterrupt(this->pinA, BoardInterruptMode_Change, PeripheralRotaryEncoder::InterruptHandler);
	Board::AttachPinInterrupt(this->pinB, BoardInterruptMode_Change, PeripheralRotaryEncoder::InterruptHandler);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralRotaryEncoder::Update()
{
	this->delta->SetInt(this->interruptDelta);
	this->count->SetInt(this->count->iValue + this->interruptDelta);
	
	this->interruptDelta = 0;
}

char PeripheralRotaryEncoder::ReadState()
{
	return (Board::DigitalRead(this->pinA) ? 2 : 0) | (Board::DigitalRead(this->pinB) ? 1 : 0);
}

void PeripheralRotaryEncoder::OnInterrupt()
{
	char state = this->ReadState();
	this->interruptDelta += this->PeripheralRotaryEncoder::statesMap[(this->state << 2) | state];
	this->state = state;
}

void PeripheralRotaryEncoder::InterruptHandler()
{
	PeripheralRotaryEncoder::interruptInstance->OnInterrupt();
}
