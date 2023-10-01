//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "SerialTransmissionChannel.h"

#include "Board.h"

#include "Log.h"

SerialTransmissionChannel::SerialTransmissionChannel(int serialIndex, int baudRate)
{
	this->serial = Board::SerialBegin(serialIndex, baudRate);
}

int SerialTransmissionChannel::AvailableInputBytes()
{
	return Board::SerialBytesAvailable(this->serial);
}

int SerialTransmissionChannel::Read(void* destination, int count)
{
	return Board::SerialRead(this->serial, destination, count);
}

int SerialTransmissionChannel::Write(const void* source, int count)
{
	return Board::SerialWrite(this->serial, source, count);
}

int SerialTransmissionChannel::Seek(int position, int seekMode)
{
	if (seekMode != SeekMode_Offset)
		return 0;

	if (position <= 0)
		return 0;

	char c;
	while ((position-- > 0) && (this->Read(&c, 1)));
}

int SerialTransmissionChannel::GetPosition()
{
	return 0;
}

void SerialTransmissionChannel::Flush()
{
	return Board::SerialFlush(this->serial);
}
