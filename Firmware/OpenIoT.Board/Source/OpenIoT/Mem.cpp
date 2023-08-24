#include "Mem.h"

#include "Math.h"

#include <string.h>

Mem::Mem(void)
{
}

Mem::~Mem(void)
{
}

void* Mem::Get(unsigned int size)
{
	return new char[size];
}

void Mem::FreeAndNull(void** data)
{
	if (*data == 0)
		return;

	delete[] * data;
	*data = 0;
}

void Mem::Free(void* data)
{
	if (data == 0)
		return;

	delete[] data;
}

void Mem::Zero(void* destination, int size)
{
	memset(destination, 0, size);
}

void Mem::Fill(void* destination, int fillSize, const void* value, int valueSize)
{
	int chunks = fillSize / valueSize;
	int remains = fillSize % valueSize;

	for (int i = chunks; i != 0; i--)
	{
		memcpy(destination, value, valueSize);
		destination = (void*)((int)destination + valueSize);
	}

	if (remains > 0)
		memcpy(destination, value, remains);
}

void Mem::Copy(void* destination, const void* source, int size)
{
	memcpy(destination, source, size);
}

int Mem::Compare(const void* block1, const void* block2, int size)
{
	return memcmp(block1, block2, size);
}

void Mem::CopyBits(const void* src, int srcBitOffset, void* dest, int destBitOffset, int bitCount)
{
	// TODO: Can be optimized to process larger (16, 32 bit) chunks while bitCount is large enough
	while (bitCount > 0)
	{
		if (srcBitOffset >= 8)
		{
			src = (void*)((unsigned int)src + (srcBitOffset >> 3));
			srcBitOffset &= 7;
		}

		if (destBitOffset >= 8)
		{
			dest = (void*)((unsigned int)dest + (destBitOffset >> 3));
			destBitOffset &= 7;
		}

		int bitsWritten = Math::Min(8 - Math::Max(srcBitOffset, destBitOffset), bitCount);

		unsigned char srcChunk = *(unsigned char*)src;
		srcChunk <<= srcBitOffset;
		srcChunk >>= destBitOffset;
		unsigned char mask = (~0);
		mask <<= 8 - bitsWritten;
		mask >>= destBitOffset;
		*(unsigned char*)dest &= (~mask);
		*(unsigned char*)dest |= (srcChunk & mask);


		srcBitOffset += bitsWritten;
		destBitOffset += bitsWritten;

		bitCount -= bitsWritten;
	}
}

void Mem::ReverseByteOrder(void* data, int size)
{
	unsigned char* lo = (unsigned char*)data;
	unsigned char* hi = (unsigned char*)data + size - 1;

	while (lo < hi)
	{
		unsigned char temp = *lo;
		*lo++ = *hi;
		*hi-- = temp;
	}
}

void Mem::CopyReversedByteOrder(void* destination, const void* source, int size)
{
	unsigned char* dst = (unsigned char*)destination;
	unsigned char* src = (unsigned char*)source + size - 1;

	while (size > 0)
	{
		*dst++ = *src--;
		size--;
	}
}
