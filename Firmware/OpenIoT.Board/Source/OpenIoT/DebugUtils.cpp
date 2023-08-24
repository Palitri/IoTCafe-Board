#include "DebugUtils.h"

#include "Log.h"

void DebugUtils::PrintChannelOutput(ITransmissionChannel* channel)
{
	const int bufferSize = 4;
	char buffer[bufferSize];
	while (channel->AvailableInputBytes() > 0)
	{
		buffer[channel->Read(buffer, bufferSize - 1)] = 0;
		Log::print(buffer);
	}
}
