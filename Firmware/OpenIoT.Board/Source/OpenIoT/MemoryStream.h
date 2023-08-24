#ifndef MemoryStream_h
#define MemoryStream_h

#include "IStream.h"

class MemoryStream :
	public IStream
{
protected:
	void *data;
	int size, capacity;
	int position;

public:
	MemoryStream();
	virtual ~MemoryStream();

	virtual int AvailableInputBytes();
	virtual int Read(void* destination, int count);
	virtual int Write(const void* source, int count);
	virtual int Seek(int position, int seekMode = SeekMode_Offset);
	virtual int GetPosition();
	virtual void Flush();

	void SetCapacity(int capacity, bool keepData = true);
	void EnsureCapacity(int capacity);
	void Clear();
};

#endif
// MemoryStream_h