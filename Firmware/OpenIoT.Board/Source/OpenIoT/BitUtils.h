#ifndef BitUtils_h
#define BitUtils_h

class BitUtils
{
public:
	static int Write7BitEncodedBytes(int src, void* dest);
	static int Read7BitEncodedBytes(void* src, int& dest);
	static int Count1Bits(void* src, int bytes);
	//  static int CountNonZeroBytes(void *src, int maxBytes = 0xFF);
};

#endif
// BitUtils_h
