#ifndef IStream_h
#define IStream_h

class IStream
{
public:
	static const int SeekMode_None = 0;
	static const int SeekMode_Offset = 1;
	static const int SeekMode_Absolute = 2;

	virtual int AvailableInputBytes() = 0;
	virtual int Read(void* destination, int count) = 0;
	virtual int Write(const void* source, int count) = 0;
	virtual int Seek(int position, int seekMode = SeekMode_Offset) = 0;
	virtual int GetPosition() = 0;
	virtual void Flush() = 0;
};

#endif
// IStream_h