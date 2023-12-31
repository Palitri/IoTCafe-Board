//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "StringStream.h"

StringStream::StringStream()
	: MemoryStream()
{
	this->manipulator = new StreamManipulator(this);
}

StringStream::~StringStream()
{
	delete this->manipulator;
}


void StringStream::WriteString(const char* text, bool nullTerminate, bool newLine, int length)
{
	this->manipulator->WriteString(text, nullTerminate, newLine, length);
}

char* StringStream::ToString()
{
	if (((char*)this->data)[this->size - 1] != 0)
	{
		this->EnsureCapacity(this->size + 1);
		((char*)this->data)[this->size] = 0;
	}

	return (char*)this->data;
}
