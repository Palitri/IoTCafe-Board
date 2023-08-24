#ifndef ChunkTransmissionProtocol_h
#define ChunkTransmissionProtocol_h

#include "ITransmissionProtocol.h"

class ChunkTransmissionProtocol
//	: public ITransmissionProtocol
{
private:
	static const int chunkIdSize = 2;
	static const int chunkFooterSize = 5;
	static const int chunkMaxDataSize = 512;
	static const int chunkMaxTotalSize = chunkMaxDataSize + chunkFooterSize;
	static const unsigned char chunkId[chunkIdSize];

	int readBufferPosition, detectedFooterOffset;
	unsigned char readBuffer[chunkMaxTotalSize];
	unsigned char chunkData[chunkFooterSize - chunkIdSize];

	unsigned short chunkCrc;
	unsigned short chunkSize;

public:
	ChunkTransmissionProtocol();
	virtual ~ChunkTransmissionProtocol(void);

	virtual int Read(void* destination, int count) = 0;
	virtual int Write(const void* source, int count) = 0;
	virtual void Feed(const void* source, int count);

	virtual void OnReceiveChunk(void* data, int dataSize) = 0;

	void BeginChunk();
	int FeedChunk(const void* data, int dataSize);
	void EndChunk();
	virtual int SendChunk(void* data, int dataSize);

	virtual void ProcessInput();
};

#endif
// ChunkTransmissionProtocol_h

