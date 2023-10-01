//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "ChunkTransmissionProtocol.h"

#include "Math.h"
#include "Mem.h"
#include "Board.h"

const unsigned char ChunkTransmissionProtocol::chunkId[2] = { 0xE6, 0x71 };

ChunkTransmissionProtocol::ChunkTransmissionProtocol()
{
	this->readBufferPosition = 0;
	this->detectedFooterOffset = 0;
}


ChunkTransmissionProtocol::~ChunkTransmissionProtocol(void)
{
}

void ChunkTransmissionProtocol::Feed(const void* source, int count)
{
	for (int i = 0; i < count; i++)
	{
		unsigned char dataByte = *(unsigned char*)source;
		source = (unsigned char*)source + 1;

		this->readBuffer[this->readBufferPosition] = dataByte;
		this->readBufferPosition++;

		if (this->detectedFooterOffset < ChunkTransmissionProtocol::chunkIdSize)
		{
			if (dataByte == ChunkTransmissionProtocol::chunkId[this->detectedFooterOffset])
				this->detectedFooterOffset++;
			else
				this->detectedFooterOffset = 0;
		}
		else if (this->detectedFooterOffset < ChunkTransmissionProtocol::chunkFooterSize)
		{
			this->chunkData[this->detectedFooterOffset - ChunkTransmissionProtocol::chunkIdSize] = dataByte;
			this->detectedFooterOffset++;

			if (this->detectedFooterOffset == ChunkTransmissionProtocol::chunkFooterSize)
			{
				this->detectedFooterOffset = 0;

				unsigned char payloadSize = this->chunkData[0];
				unsigned short chunkCrc16 = *(unsigned short*)(&this->chunkData[1]);

				int payloadPosition = this->readBufferPosition - ChunkTransmissionProtocol::chunkFooterSize - payloadSize;
				if (payloadPosition >= 0)
				{
					unsigned short actualCrc16 = Math::CRC16(&this->readBuffer[payloadPosition], payloadSize);
					if (actualCrc16 == chunkCrc16)
					{
						this->OnReceiveChunk(&this->readBuffer[payloadPosition], payloadSize);

						this->readBufferPosition = 0;
					}
				}
				else
				{
					unsigned char chunkData[ChunkTransmissionProtocol::chunkMaxDataSize];
					Mem::Copy(chunkData, this->readBuffer + ChunkTransmissionProtocol::chunkMaxTotalSize + payloadPosition, -payloadPosition);
					Mem::Copy(chunkData - payloadPosition, this->readBuffer, payloadSize + payloadPosition);
					unsigned short actualCrc16 = Math::CRC16(chunkData, payloadSize);
					if (actualCrc16 = chunkCrc16)
					{
						this->OnReceiveChunk(chunkData, payloadSize);

						this->readBufferPosition = 0;
					}
				}
			}
		}

		this->readBufferPosition %= ChunkTransmissionProtocol::chunkMaxTotalSize;
	}
}

void ChunkTransmissionProtocol::BeginChunk()
{
	this->chunkCrc = Math::CRC16Init();
	this->chunkSize = 0;
}

int ChunkTransmissionProtocol::FeedChunk(const void* data, int dataSize)
{
	this->chunkCrc = Math::CRC16Calculate(data, dataSize, this->chunkCrc);
	this->chunkSize += dataSize;

	this->Write(data, dataSize);

	return dataSize;
}

void ChunkTransmissionProtocol::EndChunk()
{
	this->Write(ChunkTransmissionProtocol::chunkId, ChunkTransmissionProtocol::chunkIdSize);
	this->Write(&this->chunkSize, 1);
	this->Write(&this->chunkCrc, 2);
}

int ChunkTransmissionProtocol::SendChunk(void* data, int dataSize)
{
	this->BeginChunk();
	int result = this->FeedChunk(data, dataSize);
	this->EndChunk();
	
	return result;
}

void ChunkTransmissionProtocol::ProcessInput()
{
	unsigned char dataByte;
	while (this->Read(&dataByte, 1) != 0)
	{
		this->Feed(&dataByte, 1);
	}
}
