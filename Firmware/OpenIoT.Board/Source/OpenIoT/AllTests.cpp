//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "AllTests.h"

#include "Log.h"

bool AllTests::Run()
{
	bool result = true;
	
	for (int i = 0; i < 10; i++)
	{
		Log::println("Waking up com");
		Board::DelayMillis(100);
	}

	Log::println("AllTests");

	result &= EepromBoardTest().Run();
	result &= EndianTest().Run();

	Log::println(result ? "AllTests successfull" : "Tesing failed");

	return result;
}