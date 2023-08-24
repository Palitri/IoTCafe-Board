#ifndef ITransmissionChannel_h
#define ITransmissionChannel_h

#include "Board.h"

#include "IStream.h"

class ITransmissionChannel :
	public IStream
{
public:
	// TODO: Either make it TransmissionChannel, or remove these implementations out of here
	virtual void WriteText(const char* text = null, bool newLine = true, int length = -1);
	virtual bool AwaitResponse(const char* text, int timeoutMillis, int length = -1);
	virtual bool WriteAwaitRetry(const char* request, const char* response, int timeoutMillis, int numTries = 1, int requestSize = -1, int responseSize = -1);
};

#endif
// ITransmissionChannel_h
