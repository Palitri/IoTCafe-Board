#include "StreamManipulator.h"

#include "Math.h"

const char* StreamManipulator::LineTermination_RN = "\r\n";
const char* StreamManipulator::LineTermination_N = "\n";

StreamManipulator::StreamManipulator(IStream* stream)
{
	this->stream = stream;
	this->lineTermination = LineTermination_RN;
}

StreamManipulator::~StreamManipulator()
{
}


void StreamManipulator::WriteInt8(char value)
{
	this->stream->Write(&value,1);
}

char StreamManipulator::ReadInt8()
{
	char result;
	this->stream->Read(&result, 1);
	return result;
}

void StreamManipulator::WriteInt16(short value)
{
	this->stream->Write(&value, 2);
};

short StreamManipulator::ReadInt16()
{
	short result;
	this->stream->Read(&result, 2);
	return result;
}

void StreamManipulator::WriteInt32(long value)
{
	this->stream->Write(&value, 4);
};

long StreamManipulator::ReadInt32()
{
	long result;
	this->stream->Read(&result, 4);
	return result;
}

void StreamManipulator::WriteUInt8(unsigned char value)
{
	this->stream->Write(&value, 1);
}

unsigned char StreamManipulator::ReadUInt8()
{
	unsigned char result;
	this->stream->Read(&result, 1);
	return result;
}

void StreamManipulator::WriteUInt16(unsigned short value)
{
	this->stream->Write(&value, 2);
};

unsigned short StreamManipulator::ReadUInt16()
{
	unsigned short result;
	this->stream->Read(&result, 2);
	return result;
}

void StreamManipulator::WriteUInt32(unsigned long value)
{
	this->stream->Write(&value, 4);
};

unsigned long StreamManipulator::ReadUInt32()
{
	unsigned long result;
	this->stream->Read(&result, 4);
	return result;
}

void StreamManipulator::WriteFloat32(float value)
{
	this->stream->Write(&value, 4);
}

float StreamManipulator::ReadFloat32()
{
	float result;
	this->stream->Read(&result, 4);
	return result;
}

void StreamManipulator::WriteBool(bool value)
{
	this->WriteUInt8(value ? 1 : 0);
}


bool StreamManipulator::ReadBool()
{
	return this->ReadUInt8() == 0 ? false : true;
}

//void StreamManipulator::WriteStringBlock(const char* value)
//{
//	int length = 0;
//	while (value[length] != 0)
//		length++;
//
//	this->WriteUInt16(length);
//	this->stream->Write(value, length);
//}
//
//char* StreamManipulator::ReadStringBlock()
//{
//	unsigned short length = this->ReadUInt16();
//	this->AllocateStringLength(length);
//	this->stream->Read(this->string, length);
//	this->string[length] = 0;
//	return this->string;
//}

void StreamManipulator::WriteString(const char* text, bool nullTerminate, bool newLine, int length)
{
	if (text != 0)
	{
		if (length == -1)
		{
			length = 0;
			while (text[length] != 0)
				length++;
		}

		this->stream->Write(text, length);
	}

	if (nullTerminate)
		this->WriteInt8(0);

	if (newLine)
	{
		char* c = (char*)this->lineTermination;
		while (*c != 0)
		{
			this->stream->Write(c, 1);
			c++;
		}
	}
}

