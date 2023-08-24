#ifndef SoftwareSerialTransmissionChannel_h
#define SoftwareSerialTransmissionChannel_h

#include "Board.h"

#include "ITransmissionChannel.h"

class SoftwareSerialTransmissionChannel :
	public ITransmissionChannel
{
private:

public:
	SoftwareSerialTransmissionChannel(int rxPin, int txPin, int baudRate);

	virtual int AvailableInputBytes();
	virtual int Read(void* destination, int count);
	virtual int Write(const void* source, int count);
	virtual int Seek(int position, int seekMode = SeekMode_Offset);
	virtual int GetPosition();
	virtual void Flush();
};

#endif
// SoftwareSerialTransmissionChannel_h
