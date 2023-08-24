#ifndef StringStream_h
#define StringStream_h

#include "MemoryStream.h"

#include "StreamManipulator.h"

class StringStream :
	public MemoryStream
{
private:
	StreamManipulator *manipulator;

public:
	StringStream();
	virtual ~StringStream();

	void WriteString(const char* text, bool nullTerminate = true, bool newLine = false, int length = -1);
	char* ToString();
};

#endif StringStream_h