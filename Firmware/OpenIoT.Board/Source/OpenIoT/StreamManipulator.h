#ifndef StreamManipulator_h
#define StreamManipulator_h

#include "IStream.h"

class StreamManipulator
{
private:
	IStream* stream;

	//char* string;
	//unsigned short stringLength;

	//void AllocateStringLength(unsigned short length);

public:
	static const char* LineTermination_RN;
	static const char* LineTermination_N;

	const char* lineTermination;

	//int capacity, length, position;

	StreamManipulator(IStream* stream);
	virtual ~StreamManipulator();

	//virtual void SetCapacity(int newCapacity) = 0;
	//virtual void Write(const void* source, int offset, int length) = 0;
	//virtual void Read(void* destination, int offset, int length) = 0;
	//virtual void Seek(int position) = 0;

	//void EnsureCapacity(int capacity);
	//void EnsureOffsetLength(int offsetLength);
	//void Clear();
	//void AppendData(void* source, int offset, int length);

	void WriteInt8(char value);
	char ReadInt8();
	void WriteInt16(short value);
	short ReadInt16();
	void WriteInt32(long value);
	long ReadInt32();

	void WriteUInt8(unsigned char value);
	unsigned char ReadUInt8();
	void WriteUInt16(unsigned short value);
	unsigned short ReadUInt16();
	void WriteUInt32(unsigned long value);
	unsigned long ReadUInt32();

	void WriteFloat32(float value);
	float ReadFloat32();

	void WriteBool(bool value);
	bool ReadBool();

	//void WriteStringBlock(const char* value);
	//char* ReadStringBlock();

	void WriteString(const char* text, bool nullTerminate = true, bool newLine = false, int length = -1);
};

#endif
// StreamManipulator_h

