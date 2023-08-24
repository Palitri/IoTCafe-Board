#include "StreamingInputDetector.h"

#include "Strings.h"
#include "Mem.h"

StreamingInputDetector::StreamingInputDetector()
{
	this->isCopy = false;
}

StreamingInputDetector::~StreamingInputDetector()
{
	this->SetMatch(null, 0, false);
}

void StreamingInputDetector::SetMatch(const char* match, int length, bool copy)
{
	if (this->isCopy)
		delete[] this->matchData;

	this->matchIndex = 0;
	this->matchLength = length == -1 ? Strings::Length(match) : length;
	this->isCopy = copy;

	if (copy)
	{
		this->matchData = new char[this->matchLength];
		Mem::Copy(this->matchData, match, this->matchLength);
	}
	else
		this->matchData = (char*)match;
}

int StreamingInputDetector::Feed(void* data, int length)
{
	for (int i = 0; i < length; i++)
	{
		char c = *(char*)data;

		if (this->matchIndex < this->matchLength)
		{
			if (c == this->matchData[this->matchIndex])
				this->matchIndex++;
			else
				this->matchIndex = c == this->matchData[0] ? 1 : 0;
		}

		if (this->matchIndex == this->matchLength)
		{
			this->matchIndex = 0;
			//this->MatchFound(data, length, i);
			return i;
		}

		data = (char*)data + 1;
	}

	return -1;
}

//void StreamingInputDetector::MatchFound(void* data, int length, int position)
//{
//
//}
