#include "PeripheralRFID.h"

#include "Math.h"

#include "Board.h"

#include "SerialTransmissionChannel.h"


PeripheralRFID::PeripheralRFID(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);

	this->transmission = null;
}

PeripheralRFID::~PeripheralRFID()
{
	if (this->transmission != null)
		delete this->transmission;
}


int PeripheralRFID::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->value = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->match = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	this->matchValue = *(unsigned short*)charCode;
	charCode += 2;

	unsigned char serialIndex =  *charCode++;

	this->transmission = new SerialTransmissionChannel(serialIndex, 9600);

	this->codeIndex = 0;
	this->timeLastRead = 0;
	this->countEmptyReads = false;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralRFID::Update()
{
	unsigned long timeRead = Board::TimeMillis();

	int bytesAvailable = this->transmission->AvailableInputBytes();
	if (bytesAvailable > 0)
	{
		this->timeLastRead = timeRead;
		this->countEmptyReads = 0;

		do
		{
			//this->transmission->Read(&this->code[this->codeIndex++], 1);

			int bytesRead = this->transmission->Read(&this->code[this->codeIndex], PeripheralRFID::codeSize - this->codeIndex);
			this->codeIndex += bytesRead;

			if (this->codeIndex == PeripheralRFID::codeSize)
			{
				this->codeIndex = 0;

				int crc = Math::CRC16(this->code, PeripheralRFID::codeSize);

				this->value->SetFloat(crc);
				this->match->SetFloat(crc == this->matchValue ? 1.0f : 0.0f);
			}

			//bytesAvailable--;
			bytesAvailable -= bytesRead;
			if (bytesAvailable <= 0)
				bytesAvailable = this->transmission->AvailableInputBytes();
		} while (bytesAvailable > 0);
	}
	else
	{
		long deltaTimeRead = timeRead - this->timeLastRead;

		if ((this->countEmptyReads > PeripheralRFID::maxEmptyReadsCount) && (deltaTimeRead > PeripheralRFID::maxEmptyReadsTime))
		{
			this->codeIndex = 0;

			this->value->SetFloat(0.0f);
			this->match->SetFloat(0.0f);
			this->timeLastRead = timeRead;
		}
		else
			this->countEmptyReads++;
	}
}
