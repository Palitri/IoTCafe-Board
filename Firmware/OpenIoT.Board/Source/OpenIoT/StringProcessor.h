#ifndef StringProcessor_h
#define StringProcessor_h

class StringProcessor
{
private:
	bool emulateNullTerminatedSegments;
	char segmentOriginalEndChar;

public:
	char* source;
	int sourceLength;
	int sourceCapacity;

	char* segment;
	int segmentLength;
	int segmentEnd;

	StringProcessor();

	/// <summary>
	/// Sets the source buffer. Very cpu light method
	/// source: The source buffer, containing the string which is to be processed
	/// sourceLength: The length of the string in the buffer. If left with value of -1, the length is determined by null termination
	/// sourceCapacity: The total capacity of the buffer. If left with value of -1, the sourceLength value is taken
	/// emulateNullTerminatedSegments: Determines whether to enable emulation of null terminated segments. Note that if the last segment needs to be null-terminated beyond sourceCapacity, null termination will not occur. If enabled, when processing each segment with the NextSegment() method, the segment will be appended with a null character, if sourceCapacity allows, and then the original character will be restored upon calling NextSegment() or ResetSegment().
	/// </summary>
	void SetSource(char* source, int sourceLength = -1, int sourceCapacity = -1, bool emulateNullTerminatedSegments = false);

	void ResetSegment();
	bool NextSegment(char separator = ',', char empty = ' ');
};

#endif
// StringProcessor_h