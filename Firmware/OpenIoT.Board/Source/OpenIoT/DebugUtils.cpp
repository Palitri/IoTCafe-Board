//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

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
