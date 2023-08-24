#ifndef EndianTest_h
#define EndianTest_h

#include "IBoardTest.h"

class EndianTest :
	public IBoardTest
{
private:
	static bool IsLittleEndian();
	static void MapEndianness();

public:
	bool Run();
};

#endif
// EndianTest_h