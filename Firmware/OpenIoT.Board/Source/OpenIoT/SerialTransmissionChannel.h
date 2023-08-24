#ifndef SerialTransmissionChannel_h
#define SerialTransmissionChannel_h

#include "ITransmissionChannel.h"

class SerialTransmissionChannel :
	public ITransmissionChannel
{
private:
	void* serial;

public:
	SerialTransmissionChannel(int serialIndex, int baudRate);

	virtual int AvailableInputBytes();
	virtual int Read(void* destination, int count);
	virtual int Write(const void* source, int count);
	virtual int Seek(int position, int seekMode = SeekMode_Offset);
	virtual int GetPosition();
	virtual void Flush();
};

#endif
// SerialTransmissionChannel_h
