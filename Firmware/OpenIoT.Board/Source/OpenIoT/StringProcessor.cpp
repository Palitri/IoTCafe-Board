#include "StringProcessor.h"

#include "Strings.h"

StringProcessor::StringProcessor()
{
	this->segmentOriginalEndChar = 0;

	this->SetSource(null, 0, 0);
}

void StringProcessor::SetSource(char* source, int sourceLength, int sourceCapacity, bool emulateNullTerminatedSegments)
{
	this->source = source;
	this->sourceLength = sourceLength >= 0 ? sourceLength : Strings::Length(source);
	this->sourceCapacity = sourceCapacity >= 0 ? sourceCapacity : this->sourceLength;
	this->emulateNullTerminatedSegments = emulateNullTerminatedSegments;

	this->ResetSegment();
}

void StringProcessor::ResetSegment()
{
	if (this->segmentOriginalEndChar != 0)
	{
		this->segment[this->segmentLength] = this->segmentOriginalEndChar;
		this->segmentOriginalEndChar = 0;
	}

	this->segment = this->source;
	this->segmentLength = 0;
	this->segmentEnd = -1;
}

bool StringProcessor::NextSegment(char separator, char empty)
{
	if ((this->emulateNullTerminatedSegments) && (this->segmentOriginalEndChar != 0))
	{
		this->segment[this->segmentLength] = this->segmentOriginalEndChar;
		this->segmentOriginalEndChar = 0;
	}

	this->segment += this->segmentEnd + 1;
	
	char *sourceEnd = this->source + this->sourceLength;
	if (this->segment >= sourceEnd)
		return false;


	int firstNonEmpty = 0;
	int lastNonEmpty = -1;
	int index = 0;
	unsigned int end = (unsigned int)sourceEnd - (unsigned int)this->segment;
	while (index <= end)
	{
		char c = this->segment[index];
		if (c == separator)
		{
			break;
		}
		else if (c != empty)
		{
			if (lastNonEmpty == -1)
				firstNonEmpty = index;

			lastNonEmpty = index;
		}

		index++;
	}
	this->segment += firstNonEmpty;
	this->segmentEnd = index - firstNonEmpty;
	this->segmentLength = lastNonEmpty - firstNonEmpty + 1;

	if ((this->emulateNullTerminatedSegments) && (this->segment + this->segmentLength < this->source + this->sourceCapacity))
	{
		this->segmentOriginalEndChar = this->segment[this->segmentLength];
		this->segment[this->segmentLength] = 0;
	}


	return true;
}
