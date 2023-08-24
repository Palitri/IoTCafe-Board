#ifndef CommandTransmissionProtocol_h
#define CommandTransmissionProtocol_h

#include "ChunkTransmissionProtocol.h"

#include "Board.h"

class CommandTransmissionProtocol
	: public ChunkTransmissionProtocol
{
public:
	CommandTransmissionProtocol();

	virtual bool OnReceiveCommand(unsigned char command, void* data, int dataSize) = 0;

	virtual void BeginCommand(unsigned char command, int dataSize);
	virtual void FeedCommand(const void* data, int dataSize);
	virtual void EndCommand();

	virtual void SendCommand(unsigned char command, const void* data = null, int dataSize = 0);

	virtual void OnReceiveChunk(void* data, int dataSize);
};

#endif
// CommandTransmissionProtocol_h

