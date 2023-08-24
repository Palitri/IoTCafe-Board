#include "EepromBoardTest.h"

#include "Log.h"

bool EepromBoardTest::Run()
{
	bool result = true;

	const int testSize = Board::eepromSize;

	Log::println("EepromBoardTest");
	Log::println("Test 1.1");
	result &= EepromBoardTest::TestEEPROMWithLog(testSize, 0, 4, true);
	Log::println("Test 1.2");
	result &= EepromBoardTest::TestEEPROMWithLog(testSize, 1, 4, true);
	Log::println("Test 1.3");
	result &= EepromBoardTest::TestEEPROMWithLog(testSize, 2, 4, true);
	Log::println("Test 1.4");
	result &= EepromBoardTest::TestEEPROMWithLog(testSize, 3, 4, true);

	Log::println("Test 2.1");
	result &= EepromBoardTest::TestEEPROMWithLog(testSize, 0, 2, true);
	Log::println("Test 2.2");
	result &= EepromBoardTest::TestEEPROMWithLog(testSize, 1, 2, true);

	Log::println("Test 3.1");
	result &= EepromBoardTest::TestEEPROMWithLog(testSize, 0, 1, true);


	Log::println("Test 4");
	Board::EEPROMWrite(13, "Hi! This is a test write/reading a bigger data block ;)", 55);
	for (int i = 13; i < 13 + 55; i++)
	{
		char c[2];
		c[1] = 0;
		Board::EEPROMRead(i, c, 1);
		Log::print(c);
	}
	Log::println();

	Log::println();

	return result;
}

int EepromBoardTest::TestEEPROMHash(int i)
{
	return i ^ (0xE6473b5a >> (i % 16));
}

void EepromBoardTest::TestEEPROMWrite(int size, int offset, int chunkSize)
{
	int TestEEPROMMask = 0xFFFFFFFF >> (32 - (chunkSize * 8));

	for (int i = offset; i <= size - chunkSize; i += chunkSize)
	{
		int valueWritten = TestEEPROMMask & TestEEPROMHash(i);
		Board::EEPROMWrite(i, &valueWritten, chunkSize);
	}
}

int EepromBoardTest::TestEEPROMRead(int size, int offset, int chunkSize)
{
	int TestEEPROMMask = 0xFFFFFFFF >> (32 - (chunkSize * 8));

	int errors = 0;

	for (int i = offset; i <= size - chunkSize; i += chunkSize)
	{
		int valueWritten = TestEEPROMMask & TestEEPROMHash(i);
		int valueRead;
		Board::EEPROMRead(i, &valueRead, chunkSize);
		valueRead = TestEEPROMMask & valueRead;

		if (valueWritten != valueRead)
		{
			Log::print("Error at ");
			Log::print(i);
			Log::print(": ");
			Log::print(valueWritten);
			Log::print(" != ");
			Log::println(valueRead);
			errors++;
			if (errors > 10)
				return i;
		}
	}

	return -1;
}

int EepromBoardTest::TestEEPROM(int size, int offset, int chunkSize, bool write)
{
	if (write)
		EepromBoardTest::TestEEPROMWrite(size, offset, chunkSize);

	return EepromBoardTest::TestEEPROMRead(size, offset, chunkSize);
}

bool EepromBoardTest::TestEEPROMWithLog(int size, int offset, int chunkSize, bool write)
{
	Log::print("EEPROM test");
	if (write)
		Log::print(" with WRITE");
	Log::print(", size ");
	Log::print(size);

	Log::print(". Bytes ");
	Log::print(chunkSize);
	Log::print(", offset ");
	Log::println(offset);
	int result = EepromBoardTest::TestEEPROM(size, offset, chunkSize, write);

	if (result != -1)
	{
		Log::print("EEPROM test Error at ");
		Log::println(result);
	}
	else
		Log::println("EEPROM test successful.");

	Log::println();

	return result == -1;
}
