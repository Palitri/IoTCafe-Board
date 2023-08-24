#ifndef RS232Protocol_h
#define RS232Protocol_h

class RS232Protocol
{
private:
	static const int BitsPerDataChunk = 8;

	char* buffer;
	int bufferSize;
	int bufferWritePos;
	int bufferReadPos;
	unsigned char bufferWriteBitPos;

	bool lastFedBitValue;
	unsigned long lastFedTimeMicros;

	unsigned int baudTimeMicros;
	
	int baudRate;
	bool endianMSB;
	unsigned char stopBitsHalves;
	bool polarityHigh;

public:
	static const int EndianMode_LSB = 0;
	static const int EndianMode_MSB = 1;

	RS232Protocol(int baudRate, bool endianMSB = false, unsigned char stopBitsHalves = 2, bool polarityHigh = true, int bufferSize = 16);
	~RS232Protocol();

	bool FeedInputBit(bool isHigh);
	int AvailableInputBytes();
	int GetInputData(char *data, int bytesRequested);

	void SetOutputData(char* data, int dataSize);
	bool BurstOutputBit(bool& outputBit);

};

#endif
// RS232Protocol_h

