//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Strings.h"

#include "Mem.h"

const char Strings::hexDigits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

char Strings::buffer[bufferSize];
int Strings::bufferIndex = 0;

Strings::Strings(void)
{
}

Strings::~Strings(void)
{
}

int Strings::ToDec(int value, char* dest, bool nullTerminate)
{
	int result = 0;

	if (value < 0)
	{
		value = -value;
		*(dest++) = '-';
		result++;
	}

	char* origin = dest;
	do
	{
		*(dest++) = hexDigits[value % 10];
		value /= 10;
		result++;
	} while (value != 0);

	char* tail = dest - 1;
	while (tail > origin)
	{
		char c = *tail;
		*tail = *origin;
		*origin = c;

		tail--;
		origin++;
	}

	if (nullTerminate)
		*dest = 0;

	return result;
}

char* Strings::ToDec(int value)
{
	char* result = Strings::GetBuffer(16);
	
	Strings::ToDec(value, result, true);

	return result;
}

int Strings::ToFloat(float value, char* dest, bool nullTerminate)
{
	int trunc = (int)value;
	int result = Strings::ToDec(trunc, dest, false);
	dest[result++] = '.';
	float frac = value - trunc;
	if (frac < 0)
		frac = -frac;
	result += Strings::ToDec((int)(frac * 10000.0f), dest + result, nullTerminate);
	return result;
}

char* Strings::ToFloat(float value)
{
	char* result = Strings::GetBuffer(32);

	Strings::ToFloat(value, result, true);

	return result;
}

int Strings::ToBool(bool value, char* dest, bool binaryOutput, bool nullTerminate)
{
	if (binaryOutput)
	{
		dest[0] = value ? '1' : '0';

		if (nullTerminate)
			dest[1] = 0;

		return 1;
	}
	else
		return Strings::Copy(dest, value ? "true" : "false", nullTerminate);
}

char* Strings::ToBool(bool value, bool binaryOutput)
{
	char* result = Strings::GetBuffer(6);

	Strings::ToBool(value, result, binaryOutput, true);

	return result;
}

int Strings::ToHex(const void* src, char* dest, int dataLength, bool nullTerminate)
{
	for (int i = 0; i < dataLength; i++)
	{
		unsigned char byteValue = *(unsigned char*)src;

		*(char*)dest = hexDigits[byteValue >> 4];
		dest++;

		*(char*)dest = hexDigits[byteValue & 0x0F];
		dest++;

		src = (unsigned char*)src + 1;
	}
	if (nullTerminate)
		*(char*)dest = 0;

	return dataLength * 2;
}

char *Strings::ToHex(const void* src, int dataLength)
{
	char* result = Strings::GetBuffer(dataLength * 2 + 1);

	Strings::ToHex(src, result, dataLength, true);

	return result;
}


void Strings::FromHex(const char* hex, void* dest, int hexLength)
{
	for (int i = 0; i < hexLength; i++)
	{
		char c = hex[i];
		char v = (c <= 57) ? c - 48 : (c <= 70) ? c - 65 + 10 : c - 97 + 10;
		if (i & 1)
		{
			*((unsigned char*)dest) = (*((unsigned char*)dest) << 4) | v;
			dest = (char*)dest + 1;
		}
		else
			*((unsigned char*)dest) = v;
	}
}

void Strings::ToBin(const void* src, char* dest, int dataLength, bool nullTerminate, const char* separator)
{
	for (int i = 0; i < dataLength; i++)
	{
		unsigned char byteValue = *(unsigned char*)src;

		for (int i = 0; i < 8; i++)
		{
			*(char*)dest = (byteValue & 0x80) ? '1' : '0';
			byteValue <<= 1;
			dest++;
		}

		src = (unsigned char*)src + 1;

		if (separator != null)
		{
			char* c = (char*)separator;
			while (*c != 0)
			{
				*(char*)dest = *c;
				dest++;
				c++;
			}
		}
	}

	if (nullTerminate)
		*(char*)dest = 0;
}

//void Strings::ToEnumeration(const void* src, char* dest, int dataLength, int chunkSize, bool nullTerminate, const char* separator)
//{
//	while (dataLength > 0)
//	{
//		int size = dataLength > chunkSize ? chunkSize : dataLength;
//		int value = 0;
//		Mem::Copy(&value, src, size);
//		src = (char*)src + size;
//
//		dest += Strings::ToDec(value, dest, false);		
//
//		if (separator != null)
//		{
//			char* c = (char*)separator;
//			while (*c != 0)
//			{
//				*(char*)dest = *c;
//				dest++;
//				c++;
//			}
//		}
//	}
//
//	if (nullTerminate)
//		*(char*)dest = 0;
//}
//
//char* Strings::ToEnumeration(const void* src, int dataLength, int chunkSize, bool nullTerminate, const char* separator)
//{
//	char* result = Strings::GetBuffer(0);
//	Strings::ToEnumeration(src, result, chunkSize, nullTerminate, separator);
//	return result;
//}

int Strings::Length(const char* str)
{
	if (str == 0)
		return 0;

	int result = 0;
	while (str[result] != 0)
		result++;

	return result;
}

int Strings::FirstIndexOf(const char* src, char c, int length)
{
	int result = 0;
	while (length != 0)
	{
		char s = *(src++);
		if (s == c)
			return result;
		else if (s == 0)
			break;
		
		length--;
		result++;
	}

	return -1;
}

int Strings::LastIndexOf(const char* src, char c, int length)
{
	if (length == -1)
	{
		int result = -1;
		for (int i = 0; src[i] != 0; i++)
		{
			if (src[i] == c)
				result = i;
		}
		
		return result;
	}
	else
	{
		while (length != 0)
		{
			length--;
			if (src[length] == c)
				return length;
		}
	}

	return -1;
}

int Strings::FirstIndexOf(const char* src, const char* fragment)
{
	char* s = (char*)src;
	while (*s != 0)
	{
		int index = 0;
		while ((s[index] == fragment[index]) && (fragment[index] != 0))
			index++;

		if (fragment[index] == 0)
			return (unsigned int)s - (unsigned int)src;

		s++;
	}

	return -1;
}


int Strings::Compare(const char* arg1, const char* arg2, int length)
{
	for (int i = 0; i < length; i++)
	{
		int diff = arg1[i] - arg2[i];
		if (diff != 0)
			return diff > 0 ? 1 : -1;
	}

	return 0;
}

bool Strings::ParseFloat(const char* src, float& result, int length)
{
	//const char* x = src;
	//result = atof(x);
	//return true;

	result = 0.0f;
	int i = 0;
	int divisor = 10;
	bool whole = true;
	char c = *src;
	while ((i < length) || ((length == -1) && (c != 0)))
	{
		if ((c >= 48) && (c <= 57))
		{
			if (whole)
				result = result * 10.0f + (c - 48);
			else
			{
				result = result + (float)(c - 48) / (float)divisor;
				divisor *= 10;
			}
		}
		else if ((c == '.') || (c == ','))
			whole = false;
		else
			return false;

		i++;
		c = src[i];
	}

	return true;
}

bool Strings::ParseInt(const char* src, int& result, int length)
{
	result = 0;
	int i = 0;
	char c = *src;
	while ((i < length) || ((length == -1) && (c != 0)))
	{
		if ((c >= 48) && (c <= 57))
		{
			result = result * 10 + (c - 48);
		}
		else
			return false;

		i++;
		c = src[i];
	}

	return true;
}

int Strings::Copy(char* dest, const char* src, bool nullTerminate)
{
	int result = 0;
	while (*src != 0)
	{
		*dest = *src;
		dest++;
		src++;
		result++;
	}
	if (nullTerminate)
		*dest = 0;

	return result;
}

char* Strings::Create(int maxLength, bool nullTerminated)
{
	if (nullTerminated)
	{
		char* result = new char[maxLength + 1];
		result[0] = 0;
		return result;
	}

	return new char[maxLength];
}

char* Strings::CreateCopy(const char* source)
{
	char* result = Strings::Create(Strings::Length(source), true);
	Strings::Copy(result, source, true);

	return result;
}

void Strings::Delete(char*& str)
{
	delete[] str;
	str = 0;
}

char* Strings::CreateMerge(const char* str1, const char* str2, bool nullTerminate)
{
	const char* args[2] = { str1, str2 };
	return Strings::CreateMerge(2, args, nullTerminate);
}

char* Strings::CreateMerge(const char* str1, const char* str2, const char* str3, bool nullTerminate)
{
	const char* args[3] = { str1, str2, str3 };
	return Strings::CreateMerge(3, args, nullTerminate);
}

char* Strings::CreateMerge(const char* str1, const char* str2, const char* str3, const char* str4, bool nullTerminate)
{
	const char* args[4] = { str1, str2, str3, str4 };
	return Strings::CreateMerge(4, args, nullTerminate);
}

char* Strings::CreateMerge(const char* str1, const char* str2, const char* str3, const char* str4, const char* str5, bool nullTerminate)
{
	const char* args[5] = { str1, str2, str3, str4, str5 };
	return Strings::CreateMerge(5, args, nullTerminate);
}

char* Strings::CreateMerge(int count, const char** str, bool nullTerminate)
{
	int length = 0;
	for (int i = 0; i < count; i++)
		length += Strings::Length(str[i]);

	char* result = Strings::Create(length, nullTerminate);
	length = 0;
	for (int i = 0; i < count; i++)
		length += Strings::Copy(&result[length], str[i], false);
	if (nullTerminate)
		result[length++] = 0;

	return result;
}

char* Strings::GetBuffer(int size)
{
	if (size > Strings::bufferSize)
		size = Strings::bufferSize;

	int index = Strings::bufferSize - Strings::bufferIndex < size ? 0 : Strings::bufferIndex;
	Strings::bufferIndex = (index + size) % Strings::bufferSize;

	return Strings::buffer + index;
}

