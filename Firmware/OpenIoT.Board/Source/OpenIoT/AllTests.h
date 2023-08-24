#ifndef AllTests_h
#define AllTests_h

#include "IBoardTest.h"

#include "EepromBoardTest.h";
#include "EndianTest.h"

class AllTests :
	public IBoardTest
{
public:
	bool Run();
};

#endif
// AllTests