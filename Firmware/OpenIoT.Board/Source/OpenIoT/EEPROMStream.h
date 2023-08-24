#ifndef EEPROMStream_h
#define EEPROMStream_h

#include "IStream.h"

class EEPROMStream
	: public IStream
{
private:
	int position;

public:
	EEPROMStream();
	virtual ~EEPROMStream();

	virtual int AvailableInputBytes();
	virtual int Read(void* destination, int count);
	virtual int Write(const void* source, int count);
	virtual int Seek(int position, int seekMode = SeekMode_Offset);
	virtual int GetPosition();
	virtual void Flush();
};

#endif
// EEPROMStream_h
