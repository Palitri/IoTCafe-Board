//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralShiftRegisterOut.h"

#include "Board.h"
#include "Math.h"

#include "Strings.h"

PeripheralShiftRegisterOut::PeripheralShiftRegisterOut(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);

	this->shiftReg = null;
	this->bitsCRC = 0;
}

PeripheralShiftRegisterOut::~PeripheralShiftRegisterOut()
{
	if (this->shiftReg != null)
		delete this->shiftReg;
}


int PeripheralShiftRegisterOut::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->pBits = this->AddProperty(new Property((void**)&charCode, PropertyType_Data, PropertyFlag_Read | PropertyFlag_Write));

	unsigned char pinSerial = *charCode++;
	unsigned char pinClock = *charCode++;
	unsigned char pinStorage = *charCode++;

	this->bitCount = *(int*)charCode;
	charCode += 4;

	this->shiftReg = new ShiftRegister(pinSerial, pinClock, pinStorage);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralShiftRegisterOut::Update()
{
	int dataSize = this->pBits->Size();
	unsigned short bitsCRC = Math::CRC16((void*)this->pBits->data, dataSize);
	if (bitsCRC != this->bitsCRC)
	{
		int bitCount = Math::Min(this->bitCount, dataSize * 8);

		this->shiftReg->SetBits(this->pBits->data, bitCount);

		this->bitsCRC = bitsCRC;
	}
}

void PeripheralShiftRegisterOut::ProcessCommand(char code, const char* data, int size)
{
	switch (code)
	{
		 case PeripheralShiftRegisterOut::CommandCode_SetBits:
		 {
			this->pBits->SetDataValue((void*)data, size);
			this->Update();
			break;
		 }
	}
}