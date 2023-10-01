//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "GenericIRProtocol.h"

#include "BitUtils.h"
#include "Math.h"

#include "Log.h"

GenericIRProtocol::GenericIRProtocol(void* data)
{
	this->Configure(data);
}

GenericIRProtocol::~GenericIRProtocol()
{
}

int GenericIRProtocol::Configure(void* data)
{
	unsigned char* charData = (unsigned char*)data;

	int configFlags = 0;
	if (charData != null)
		charData += BitUtils::Read7BitEncodedBytes(charData, configFlags);

	this->initialBurstHighMicros = configFlags & (1 << configBit_InitialBurstHigh) ? *charData++ * 64 : GenericIRProtocol::configDefaultBurstHighMicros;

	this->zeroHighMicros = configFlags & (1 << configBit_ZeroHigh) ? *charData++ * 16 : this->initialBurstHighMicros / 16;

	this->oneHighMicros = configFlags & (1 << configBit_OneHigh) ? *charData++ * 16 : this->zeroHighMicros;

	this->finalHighMicros = configFlags & (1 << configBit_FinalHigh) ? *charData++ * 16 : this->zeroHighMicros;

	this->repeatInitialHighMicros = configFlags & (1 << configBit_RepeatInitialHigh) ? *charData++ * 64 : this->initialBurstHighMicros;

	this->repeatOneHighMicros = configFlags & (1 << configBit_RepeatOneHigh) ? *charData++ * 64 : this->zeroHighMicros;

	this->dataBitsCount = configFlags & (1 << configBit_DataBitsCount) ? *charData++ : 0;

	this->initialBurstLowMicros = configFlags & (1 << configBit_InitialBurstLow) ? *charData++ * 64 : this->initialBurstHighMicros / 2;

	this->zeroLowMicros = configFlags & (1 << configBit_ZeroLow) ? *charData++ * 16 : this->zeroHighMicros;

	this->oneLowMicros = configFlags & (1 << configBit_OneLow) ? *charData++ * 16 : this->zeroHighMicros * 3;

	this->repeatInitialLowMicros = configFlags & (1 << configBit_RepeatInitialLow) ? *charData++ * 64 : this->repeatInitialHighMicros / 4;

	this->repeatIntervalMicros = configFlags & (1 << configBit_RepeatInterval) ? *charData++ * 4000 : this->initialBurstHighMicros * 12;

	this->repeatsFullData = configFlags & (1 << configBit_RepeatsFullData);

	this->inversedDataPulse = configFlags & (1 << configBit_InversedDataPulse);

	this->carrierFrequencyHz = configFlags & (1 << configBit_CarrierFrequency) ? (*charData++) * 1000 : GenericIRProtocol::configDefaultCarrierFrequeny;

	this->effectiveRepeatIntervalMicros = this->repeatIntervalMicros;
	this->carrierFrequencyHalfTimeMicros = 500000 / this->carrierFrequencyHz;

	//Log::println(initialBurstHighMicros);
	//Log::println(initialBurstLowMicros);
	//Log::println(zeroHighMicros);
	//Log::println(zeroLowMicros);
	//Log::println(oneHighMicros);
	//Log::println(oneLowMicros);
	//Log::println(finalHighMicros);
	//Log::println(dataBitsCount);
	//Log::println(repeatInitialHighMicros);
	//Log::println(repeatInitialLowMicros);
	//Log::println(repeatOneHighMicros);
	//Log::println(repeatsFullData);
	//Log::println(inversedDataPulse);
	//Log::println(repeatIntervalMicros);
	//Log::println(carrierFrequencyHz);
	//Log::println(carrierFrequencyHalfTimeMicros);

	return (unsigned int)charData - (unsigned int)data;
}

void GenericIRProtocol::Encode(int pin, void* data, unsigned char dataLength)
{
	unsigned char* codeBytes = (unsigned char*)data;
	unsigned char bitCount = this->dataBitsCount == 0 ? dataLength * 8 : this->dataBitsCount;

	this->effectiveRepeatIntervalMicros = Math::Max(this->repeatIntervalMicros, this->initialBurstHighMicros * 4 + this->zeroHighMicros * 4 * bitCount);

	this->Burst(pin, this->initialBurstHighMicros, true);
	this->Burst(pin, this->initialBurstLowMicros, false);

	for (int i = 0; i < dataLength; i++)
	{
		unsigned char c = codeBytes[i];
		for (int b = 0; b < 8; b++)
		{
			if (bitCount == 0)
				break;
			bitCount--;

			if ((c & 0x80) != 0)
			{
				this->Burst(pin, this->oneHighMicros, !this->inversedDataPulse);
				this->Burst(pin, this->oneLowMicros, this->inversedDataPulse);
			}
			else
			{
				this->Burst(pin, this->zeroHighMicros, !this->inversedDataPulse);
				this->Burst(pin, this->zeroLowMicros, this->inversedDataPulse);
			}

			c <<= 1;
		}
	}

	this->Burst(pin, this->finalHighMicros, !this->inversedDataPulse);
}

void GenericIRProtocol::Repeat(int pin, void* data, unsigned char dataLength)
{
	if (this->repeatsFullData)
		this->Encode(pin, data, dataLength);
	else
	{
		this->Burst(pin, this->repeatInitialHighMicros, true);
		this->Burst(pin, this->repeatInitialLowMicros, false);

		this->Burst(pin, this->repeatOneHighMicros, true);
	}
}

int GenericIRProtocol::GetRepeatTimeMicros()
{
	return this->effectiveRepeatIntervalMicros;
}

void GenericIRProtocol::Burst(int pin, int timeMicros, bool high)
{
	if (high)
	{
		Board::Pulse(pin, timeMicros, this->carrierFrequencyHz);
		//    int halfTime = this->carrierFrequencyHalfTimeMicros - 1;
		//    unsigned long time = Board::TimeMicros();
		//    while (Board::TimeMicros() - time < timeMicros)
		//    //while (timeMicros > 0)
		//    {
		//      Board::DigitalWrite(pin, true);
		//      Board::DelayMicros(halfTime);
		//      //timeMicros -= halfTime;
		//      
		//      Board::DigitalWrite(pin, false);
		//      Board::DelayMicros(halfTime);
		//      //timeMicros -= halfTime;
		//    }
	}
	else
		Board::DelayMicros(timeMicros);
}
