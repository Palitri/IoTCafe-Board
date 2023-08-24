#ifndef Uid128_h
#define Uid128_h

class Uid128
{
public:
	static const unsigned char size = 16;

	unsigned char data[size];

	static int FromBytes(void* uid, const void* src);

	static int FromString(void* uid, const char* src);
	static int ToString(const void* uid, char* dest, bool nullTerminate = true);
	static char* ToString(const void* uid);
};

#endif
// Uid128_h