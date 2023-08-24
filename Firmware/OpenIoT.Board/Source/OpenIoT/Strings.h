#ifndef Strings_h
#define Strings_h

#include "Board.h"

class Strings
{
private:
	// TODO: Get rid of this
	static const int bufferSize = 2048;
	static char buffer[bufferSize];
	static int bufferIndex;

public:
	static const char hexDigits[16];

	Strings(void);
	~Strings(void);

	static int ToDec(int value, char* dest, bool nullTerminate = true);
	static char* ToDec(int value);
	
	static int ToFloat(float value, char* dest, bool nullTerminate = true);
	static char* ToFloat(float value);
	
	static int ToBool(bool value, char* dest, bool binaryOutput = true, bool nullTerminate = true);
	static char* ToBool(bool value, bool binaryOutput = true);
	
	static int ToHex(const void* src, char* dest, int dataLength, bool nullTerminate = true);
	static char* ToHex(const void* src, int dataLength);
	static void FromHex(const char* hex, void* dest, int hexLength);
	
	static void ToBin(const void* src, char* dest, int dataLength, bool nullTerminate = true, const char* separator = null);

	//static void ToEnumeration(const void* src, char* dest, int dataLength, int chunkSize = 1, bool nullTerminate = true, const char* separator = null);
	//static char* ToEnumeration(const void* src, int dataLength, int chunkSize = 1, bool nullTerminate = true, const char* separator = null);

	static int Length(const char* str);

	static int FirstIndexOf(const char* src, char c, int length = -1);
	static int LastIndexOf(const char* src, char c, int length = -1);

	static int FirstIndexOf(const char* src, const char* fragment);

	static int Compare(const char* arg1, const char* arg2, int length);

	static bool ParseFloat(const char* src, float& result, int length = -1);
	static bool ParseInt(const char* src, int& result, int length = -1);

	static int Copy(char* dest, const char* src, bool nullTerminate = true);

	static char* Create(int maxLength = 32, bool nullTerminated = true);
	static char* CreateCopy(const char* source);
	static char* CreateMerge(const char* str1, const char* str2, bool nullTerminate = true);
	static char* CreateMerge(const char* str1, const char* str2, const char* str3, bool nullTerminate = true);
	static char* CreateMerge(const char* str1, const char* str2, const char* str3, const char* str4, bool nullTerminate = true);
	static char* CreateMerge(const char* str1, const char* str2, const char* str3, const char* str4, const char* str5, bool nullTerminate = true);
	static char* CreateMerge(int count, const char** str, bool nullTerminate = true);
	static void Delete(char*& str);

	// Gets a temporary buffer from pre-allocated memory. Buffer memory is recycled, and might intersect with another buffer produced by the method, if enough memory has been requested
	static char* GetBuffer(int size = 32);
	//// Gets a temporary buffer (see GetBuffer()) of yet undetermined size (at least minSize or more). Needs to pair with EndBuffer() after work with the buffer is done. Multiple calls of BeginBuffer() would yield the same address if each call is not accompanied by a respective EndBuffer()
	//static char* BeginBuffer(int minSize = 256);
	//static char* EndBuffer(int size);

	//static int 
};

#endif
// Strings_h
