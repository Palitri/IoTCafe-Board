#include "Uid128.h"

#include "Strings.h"
#include "Mem.h"

int Uid128::FromBytes(void* uid, const void* src)
{
	Mem::Copy(uid, src, Uid128::size);
	return Uid128::size;
}

int Uid128::FromString(void* uid, const char* src)
{
	Strings::FromHex(src, uid, Uid128::size * 2);
	return Uid128::size;
}

int Uid128::ToString(const void* uid, char* dest, bool nullTerminate)
{
	return Strings::ToHex(uid, dest, Uid128::size, nullTerminate);
}

char* Uid128::ToString(const void* uid)
{
	return Strings::ToHex(uid, Uid128::size);
}
