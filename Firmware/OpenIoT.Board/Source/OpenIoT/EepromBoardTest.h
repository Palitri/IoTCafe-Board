#ifndef EepromBoardTest_h
#define EepromBoardTest_h

#include "IBoardTest.h"

class EepromBoardTest :
	public IBoardTest
{
private:
	static int TestEEPROMHash(int i);
	static void TestEEPROMWrite(int size, int offset, int chunkSize);
	static int TestEEPROMRead(int size, int offset, int chunkSize);
	static int TestEEPROM(int size, int offset, int chunkSize, bool write);
	static bool TestEEPROMWithLog(int size, int offset, int chunkSize, bool write);

public:
	bool Run();
};

#endif
// EepromBoardTest_h