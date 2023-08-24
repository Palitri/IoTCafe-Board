#include "RS232Protocol.h"

#include "Board.h"

RS232Protocol::RS232Protocol(int baudRate, bool endianMSB, unsigned char stopBitsHalves, bool polarityHigh, int bufferSize)
{
	this->baudRate = baudRate;
	this->endianMSB = endianMSB;
	this->stopBitsHalves = stopBitsHalves;
	this->bufferSize = bufferSize;
	this->polarityHigh = polarityHigh;

	this->buffer = new char[this->bufferSize];
	this->bufferReadPos = 0;
	this->bufferWritePos = 0;
	this->bufferWriteBitPos = 0;

	this->baudTimeMicros = 1000000 / this->baudRate;

	this->lastFedBitValue = this->polarityHigh;
	this->lastFedTimeMicros = Board::TimeMicros();
}

RS232Protocol::~RS232Protocol()
{
	delete[] this->buffer;
}



bool RS232Protocol::FeedInputBit(bool isHigh)
{
	if (isHigh != this->lastFedBitValue)
	{
		unsigned long time = Board::TimeMicros();
		unsigned long deltaTime = time - this->lastFedTimeMicros;

		int baudCount = deltaTime / this->baudTimeMicros;
		if (isHigh)
		{

		}
		else
		{
			if (this->bufferWriteBitPos == 0)
			{

			}
		}




		this->lastFedBitValue = isHigh;
		this->lastFedTimeMicros = time;
	}

	return false;
}

int RS232Protocol::AvailableInputBytes()
{
	return this->bufferReadPos < this->bufferWritePos;
}

int RS232Protocol::GetInputData(char* data, int bytesRequested)
{
	int bytesAvailable = this->bufferReadPos < this->bufferWritePos;

	int bytesRead = 0;
	int bufferReadEndPos = this->bufferWritePos == 0 ? this->bufferSize - 1 : this->bufferWritePos;
	while ((this->bufferReadPos != bufferReadEndPos) && (bytesRead < bytesRequested))
	{
		*data = this->buffer[this->bufferReadPos];
		data++;

		this->bufferReadPos = (this->bufferReadPos + 1) % this->bufferSize;
		bytesRead++;
	}

	return bytesRead;
}



void RS232Protocol::SetOutputData(char* data, int dataSize)
{

}

bool RS232Protocol::BurstOutputBit(bool& outputBit)
{
	return false;
}
