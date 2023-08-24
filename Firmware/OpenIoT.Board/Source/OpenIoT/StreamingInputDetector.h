#ifndef StreamingInputDetector_h
#define StreamingInputDetector_h

class StreamingInputDetector
{
private:
	char* matchData;
	int matchIndex, matchLength;
	bool isCopy;

public:
	StreamingInputDetector();
	~StreamingInputDetector();

	void SetMatch(const char* match, int length = -1, bool copy = false);

	int Feed(void* data, int length);

	//virtual void MatchFound(void* data, int length, int position);
};

#endif
// StreamingInputDetector_h
