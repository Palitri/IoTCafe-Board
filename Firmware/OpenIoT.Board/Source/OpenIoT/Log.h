#ifndef Log_h
#define Log_h

#include "ITransmissionChannel.h"

#include "Board.h"

class Log
{
private:
	static ITransmissionChannel* receiver;
	static char buffer[16];

public:
	static void SetTransmissionChannel(ITransmissionChannel* receiver);

	static void print(const char* text = null, int length = -1);
	static void print(int value);
	static void print(float value, int decimals = 2);

	static void println(const char* text = null, int length = -1);
	static void println(int value);
	static void println(float value, int decimals = 2);
};
#endif
// Log_h
