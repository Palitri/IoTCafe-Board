//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "EndianTest.h"

#include "Log.h"
#include "Strings.h"

bool EndianTest::Run()
{
	bool result = EndianTest::IsLittleEndian();

	Log::println("EndianTest ");
	Log::print(result ? "Little Endian" : "Big Endian");
	EndianTest::MapEndianness();

	Log::println();

	return result;
}

bool EndianTest::IsLittleEndian()
{
	union
	{
		unsigned char u8;
		unsigned short u16;
		unsigned int u32;
	} u;
	
	u.u32 = 0x4A;

	return (u.u8 == u.u16) && (u.u8 == u.u32);
}

void EndianTest::MapEndianness()
{
	unsigned char buffer[4];
	buffer[0] = 0x01;
	buffer[1] = 0x02;
	buffer[2] = 0x03;
	buffer[3] = 0x04;

	Log::print("Bytes: ");
	Log::print(Strings::ToHex(buffer + 0, 1));
	Log::print(", ");
	Log::print(Strings::ToHex(buffer + 1, 1));
	Log::print(", ");
	Log::print(Strings::ToHex(buffer + 2, 1));
	Log::print(", ");
	Log::println(Strings::ToHex(buffer + 3, 1));

	unsigned char u8;
	unsigned short u16;
	Log::print("16 bits: ");
	for (int i = 0; i < 2; i++)
	{
		if (i != 0)
			Log::print(", ");

		u16 = *(unsigned short*)&buffer[i * 2];
		u8 = u16 >> 8;
		Log::print(Strings::ToHex(&u8, 1));
		u8 = u16;
		Log::print(Strings::ToHex(&u8, 1));
	}
	Log::println();


	unsigned int u32;
	u32 = *(unsigned int*)&buffer[0];
	Log::print("32 bits: ");
	u8 = u32 >> 24;
	Log::print(Strings::ToHex(&u8, 1));
	u8 = u32 >> 16;
	Log::print(Strings::ToHex(&u8, 1));
	u8 = u32 >> 8;
	Log::print(Strings::ToHex(&u8, 1));
	u8 = u32;
	Log::println(Strings::ToHex(&u8, 1));
}