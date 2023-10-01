//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralIROutput.h"

#include "Board.h"
#include "Math.h"
#include "Strings.h"

PeripheralIROutput::PeripheralIROutput(IClusterDevice* device) :
	Peripheral(device)
{
	this->irProtocol = null;

	this->SetPropertiesCapacity(3);
}

PeripheralIROutput::~PeripheralIROutput()
{
	if (this->irProtocol != null)
		delete this->irProtocol;
}


int PeripheralIROutput::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->active = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->code = this->AddProperty(new Property((void**)&charCode, PropertyType_Data, PropertyFlag_Read));
	this->configuration = this->AddProperty(new Property((void**)&charCode, PropertyType_Data, PropertyFlag_Read));

	this->pin = *charCode++;

	if (this->irProtocol != null)
		delete this->irProtocol;
	this->irProtocol = IRProtocol::Dispatch(this->configuration->data);

	this->timeLastOutput = 0;
	this->isRepeating = false;

	Board::SetPinMode(this->pin, BoardPinMode_DigitalOutput);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralIROutput::Update()
{
	if (this->irProtocol == null)
		return;

	if (this->active->bValue)
	{
		unsigned int time = Board::TimeMicros();
		if (!this->isRepeating)
		{
			this->irProtocol->Encode(this->pin, this->code->data, this->code->Size());

			this->isRepeating = true;
			this->timeLastOutput = time;
		}
		else
		{
			if (time - this->timeLastOutput > this->irProtocol->GetRepeatTimeMicros())
			{
				this->irProtocol->Repeat(this->pin, this->code->data, this->code->Size());

				this->timeLastOutput = time;
			}
		}
	}
	else
		this->isRepeating = false;
}

//Volume up : 400555AA
//Power : 028C99400501FE
//AC config : 0103341F
//AC Power ON Low : 28C60008087F900C8DC020000000041F
//AC Power ON High : 28C60008487F900C8DC08000000004DF
//AC Power On, 23C, Quiet, Heat : 28C60008087F900C8E202000800004EC
//AC Low : 28C60008087F900C0DC020000000049F
//AC High : 28C60008087F900C0DC080000000043F = 01033474, 28C60008087F900C0DC080000000043F
//AC Power OFF : 28C600080840BF
//AC Swing : 28C600080836C9
//Sony config : id: 01 + flags : 0b10000001 11110101 01001011 = 0x81F54B + data : 2723000C004E1028 = 0181F54B 2723000C004E0B28
//Sony On : 101010010000, A900