//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "BitUtils.h"

int BitUtils::Write7BitEncodedBytes(int src, void* dest)
{
	int byteCount = 0;

	do
	{
		byteCount++;
		*(unsigned char*)dest = (unsigned char)src;
		src >>= 7;
		if (src == 0)
		{
			*(unsigned char*)dest &= 0x7F;
			return byteCount;
		}
		else
			*(unsigned char*)dest |= 0x80;

		dest = (unsigned char*)dest + 1;
	} while (true);

	return byteCount;
}

int BitUtils::Read7BitEncodedBytes(void* src, int& dest)
{
	int bytesRead = 0;
	unsigned char bits;
	dest = 0;
	do
	{
		bits = *(unsigned char*)src;
		dest <<= 7;
		dest |= bits & 0x7F;

		src = (unsigned char*)src + 1;

		bytesRead++;
	} while ((bits & 0x80) != 0);

	return bytesRead;
}

int BitUtils::Count1Bits(void* src, int bytes)
{
	int result = 0;
	while (bytes > 0)
	{
		unsigned char byteValue = *(unsigned char*)src;
		while (byteValue != 0)
		{
			result += byteValue & 1;
			byteValue >>= 1;
		}
	}

	return result;
}

//int BitUtils::CountNonZeroBytes(void *src, int maxBytes)
//{
//  int result = 0;
//  while ((*(unsigned char*)src) != 0)
//  {
//    src += 1;
//    result++;
//  }
//
//  return result;
//}
//
