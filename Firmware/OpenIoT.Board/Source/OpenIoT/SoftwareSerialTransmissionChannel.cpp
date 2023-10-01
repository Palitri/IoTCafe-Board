//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "SoftwareSerialTransmissionChannel.h"

#include "Strings.h"

SoftwareSerialTransmissionChannel::SoftwareSerialTransmissionChannel(int rxPin, int txPin, int baudRate)
{
}


int SoftwareSerialTransmissionChannel::AvailableInputBytes()
{
	return 0;
}

int SoftwareSerialTransmissionChannel::Read(void* destination, int count)
{
	return 0;
}

int SoftwareSerialTransmissionChannel::Write(const void* source, int count)
{
	return 0;
}


int SoftwareSerialTransmissionChannel::Seek(int position, int seekMode)
{
	if (seekMode != SeekMode_Offset)
		return 0;

	if (position <= 0)
		return 0;

	char c;
	while ((position > 0) && (this->Read(&c, 1)))
	{
		position--;
	}

	return position;
}

int SoftwareSerialTransmissionChannel::GetPosition()
{
	return 0;
}

void SoftwareSerialTransmissionChannel::Flush()
{
}
