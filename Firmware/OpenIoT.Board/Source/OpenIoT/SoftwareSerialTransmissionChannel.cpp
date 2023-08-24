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
