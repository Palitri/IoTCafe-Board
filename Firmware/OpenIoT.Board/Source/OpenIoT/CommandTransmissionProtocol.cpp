#include "CommandTransmissionProtocol.h"

CommandTransmissionProtocol::CommandTransmissionProtocol() :
	ChunkTransmissionProtocol()
{

}

void CommandTransmissionProtocol::BeginCommand(unsigned char command, int dataSize)
{
	this->BeginChunk();
	this->FeedChunk(&command, 1);
	this->FeedChunk(&dataSize, 1);
}

void CommandTransmissionProtocol::FeedCommand(const void* data, int dataSize)
{
	this->FeedChunk(data, dataSize);
}

void CommandTransmissionProtocol::EndCommand()
{
	this->EndChunk();
}


void CommandTransmissionProtocol::SendCommand(unsigned char command, const void* data, int dataSize)
{
	this->BeginCommand(command, dataSize);
	this->FeedCommand(data, dataSize);
	this->EndCommand();
}

void CommandTransmissionProtocol::OnReceiveChunk(void* data, int dataSize)
{
	void* dataEnd = (unsigned char*)data + dataSize;
	while (data < dataEnd)
	{
		unsigned char commandCode = *(unsigned char*)data;
		data = (unsigned char*)data + 1;

		unsigned char commandDataSize = *(unsigned char*)data;
		data = (unsigned char*)data + 1;

		this->OnReceiveCommand(commandCode, data, commandDataSize);

		data = (unsigned char*)data + commandDataSize;
	}
}
