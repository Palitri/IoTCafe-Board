//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralIRInput.h"

#include "Board.h"
#include "Math.h"
#include "Strings.h"
#include "Mem.h"

#include "Log.h"

PeripheralIRInput* PeripheralIRInput::interruptInstance;

PeripheralIRInput::PeripheralIRInput(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(3);
}

PeripheralIRInput::~PeripheralIRInput()
{

}


int PeripheralIRInput::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->value = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->code = this->AddProperty(new Property((void**)&charCode, PropertyType_Data, PropertyFlag_Read));
	this->configuration = this->AddProperty(new Property((void**)&charCode, PropertyType_Data, PropertyFlag_Read));

	this->pin = *charCode++;


	this->Reset();
	this->timeLastRead = 0;

	Board::SetPinMode(this->pin, BoardPinMode_DigitalInput);

	PeripheralIRInput::interruptInstance = this;
	Board::AttachPinInterrupt(this->pin, BoardInterruptMode_Change, PeripheralIRInput::InterruptHandler);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralIRInput::Update()
{
	unsigned long time = Board::TimeMicros();
	if (time < this->timeLastRead)
	{
		this->timeLastRead = time;
		return;
	}
	long idleTime = time - this->timeLastRead;

	this->sequenceReceived |= (this->bitIndex > 0) && (idleTime > this->timeInitialSequence);
	if (this->sequenceReceived)
	{
		if (this->bitIndex >= 4)
		{
			unsigned short bitsByteLength = this->bitIndex >> 3;
			this->value->SetFloat(this->GetBitsHash());
			//this->code->SetDataValue((void*)this->bits, bitsByteLength);
			this->CreateIRCode();

			Log::print(this->bitIndex);
			Log::print(", ");
			Log::print((int)this->value->fValue);
			Log::print(", ");
			char text[50];
			Strings::ToHex((void*)this->code->data, text, this->code->Size(), true);
			Log::print(text);
			Log::print(", ");
			Strings::ToBin((void*)this->bits, text, 4, true, " ");
			Log::println(text);
		}

		this->Reset();
	}

	bool inputEnded = idleTime > PeripheralIRInput::minIdleTimeBetweenPacketsMicros;
	if (inputEnded)
	{
		const unsigned char nil = 0;
		this->value->SetFloat(0.0f);
		this->code->SetDataValue((void*)&nil, 1);

		this->Reset();

		return;
	}
}

void PeripheralIRInput::CreateIRCode()
{
	unsigned short bitsByteLength = this->bitIndex >> 3;

	this->code->SetDataValue((void*)this->bits, bitsByteLength);

	if ((Math::Abs(PeripheralIRInput::defaultInitialBurstHighMicros - (int)this->timeInitialSequence) < 8 * 64) &&
		(Math::Abs(PeripheralIRInput::defaultBitBurstHighMicros - (int)this->timeZeroBitPulse) < 3 * 16))
	{
		this->configuration->SetDataSize(0);
	}
	else
	{
		this->configuration->SetDataSize(4);
		*(unsigned char*)((unsigned char*)(this->configuration->data) + 0) = 0x01;
		*(unsigned char*)((unsigned char*)(this->configuration->data) + 1) = 0x03;
		*(unsigned char*)((unsigned char*)(this->configuration->data) + 2) = this->timeInitialSequence / 64;
		*(unsigned char*)((unsigned char*)(this->configuration->data) + 3) = this->timeZeroBitPulse / 16;
		this->configuration->isChanged = true;
	}
}


void PeripheralIRInput::OnInterrupt()
{
	unsigned long time = Board::TimeMicros();
	if (time < this->timeLastRead)
	{
		this->timeLastRead = time;
		return;
	}
	long pulseTime = time - this->timeLastRead;
	this->timeLastRead = time;
	bool high = Board::DigitalRead(this->pin);

	if (high)
	{
		if (this->bitIndex == -1)
		{
			this->timeInitialSequence = pulseTime;
		}

		else if (this->bitIndex == 0)
		{
			this->timeZeroBitPulse = pulseTime;
			this->timeMaxZeroBitPulse = pulseTime * 2;
		}
	}
	else
	{
		bool received = this->sequenceReceived;
		this->sequenceReceived |= (this->bitIndex > 0) && (pulseTime > this->timeInitialSequence);

		if (!this->sequenceReceived)
		{
			if ((this->bitIndex >= 0) && (this->bitIndex < PeripheralIRInput::maxBits))
			{
				volatile unsigned char* bitsByte = &(this->bits[this->bitIndex >> 3]);
				*bitsByte <<= 1;
				*bitsByte |= pulseTime > this->timeMaxZeroBitPulse;
			}

			this->bitIndex++;
		}
	}
}

void PeripheralIRInput::InterruptHandler()
{
	PeripheralIRInput::interruptInstance->OnInterrupt();
}

void PeripheralIRInput::Reset()
{
	for (int i = 0; i < PeripheralIRInput::maxBitBytes; i++)
		this->bits[i] = 0;

	this->bitIndex = -2;
	this->sequenceReceived = false;

	this->timeInitialSequence = 0;
}

unsigned short PeripheralIRInput::GetBitsHash()
{
	return Math::CRC16((void*)this->bits, PeripheralIRInput::maxBitBytes);
}
