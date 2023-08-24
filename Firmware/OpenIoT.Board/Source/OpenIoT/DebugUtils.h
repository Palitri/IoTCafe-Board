#ifndef DebugUtils_h
#define DebugUtils_h

#include "ITransmissionChannel.h"

#include "Board.h"

class DebugUtils
{
public:

	static void PrintChannelOutput(ITransmissionChannel* channel);
};

#endif
// DebugUtils_h