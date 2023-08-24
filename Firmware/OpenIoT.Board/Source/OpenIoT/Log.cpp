#include "Log.h"

#include "Strings.h"
#include "Math.h"

ITransmissionChannel* Log::receiver = null;
char Log::buffer[16];

void Log::SetTransmissionChannel(ITransmissionChannel* receiver)
{
	Log::receiver = receiver;
}

void Log::print(const char* text, int length)
{
	if (text == null)
		return;

	if (Log::receiver != null)
		if (length == -1)
			Log::receiver->WriteText(text, false);
		else
			Log::receiver->WriteText(text, false, length);
}

void Log::print(int value)
{
	Strings::ToDec(value, Log::buffer, true);
	Log::print(Log::buffer);
}

void Log::print(float value, int decimals)
{
	int trunc = (int)value;
	Log::print((int)value);
	Log::print(".");
	Log::print((int)((value - trunc) * Math::Power(10, decimals)));
}

void Log::println(const char* text, int length)
{
	if (Log::receiver != null)
		if (text == null)
			Log::receiver->WriteText(null, true, 0);
		else
		{
			if (length == -1)
				Log::receiver->WriteText(text, true);
			else
				Log::receiver->WriteText(text, true, length);
		}
}

void Log::println(int value)
{
	Strings::ToDec(value, Log::buffer, true);
	Log::println(Log::buffer);
}

void Log::println(float value, int decimals)
{
	int trunc = (int)value;
	Log::print((int)value);
	Log::print(".");
	Log::println((int)((value - trunc) * Math::Power(10, decimals)));
}
