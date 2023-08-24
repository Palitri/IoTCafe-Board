#include "ITransmissionChannel.h"

#include "Strings.h"
//#include "Log.h"

void ITransmissionChannel::WriteText(const char* text, bool newLine, int length)
{
	if (text != null)
	{
		if (length == -1)
			length = Strings::Length(text);

		this->Write(text, length);
	}

	if (newLine)
		this->Write("\r\n", 2);
}

bool ITransmissionChannel::AwaitResponse(const char* text, int timeoutMillis, int length)
{
	int index = 0;

	unsigned long time = Board::TimeMillis();
	while (Board::TimeMillis() - time < timeoutMillis)
	{
		if ((index < length) || ((length == -1) && (text[index] != 0)))
		{
			if (this->AvailableInputBytes() > 0)
			{
				char c;
				this->Read(&c, 1);
				//Log::print(&c, 1);
				if (c == text[index])
					index++;
				else
					index = c == text[0] ? 1 : 0;
			}
		}
		else
			return true;
	}

	return false;
}

bool ITransmissionChannel::WriteAwaitRetry(const char* request, const char* response, int timeoutMillis, int numTries, int requestSize, int responseSize)
{
	while (numTries > 0)
	{
		this->WriteText(request, true, requestSize);

		if (this->AwaitResponse(response, timeoutMillis, responseSize))
			return true;

		numTries--;
	}

	return false;
}