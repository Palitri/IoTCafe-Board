#include "EEPROMStream.h"

#include "Math.h"

#include "Board.h"

EEPROMStream::EEPROMStream()
{
	this->position = 0;
}

EEPROMStream::~EEPROMStream()
{
}


int EEPROMStream::AvailableInputBytes()
{
	return Board::eepromSize - this->position;
}

int EEPROMStream::Read(void* destination, int count)
{
	int bytesRead = Board::EEPROMRead(this->position, destination, count);
	this->position += bytesRead;

	return bytesRead;
}


int EEPROMStream::Write(const void* source, int count)
{
	int bytesWritten = Board::EEPROMWrite(this->position, source, count);
	this->position += bytesWritten;

	return bytesWritten;
}

int EEPROMStream::Seek(int position, int seekMode)
{
	switch (seekMode)
	{
		case SeekMode_Absolute:
			this->position = position;
			break;

		case SeekMode_Offset:
			this->position += position;
	}

	return position;
}

int EEPROMStream::GetPosition()
{
	return this->position;
}

void EEPROMStream::Flush()
{
	Board::EEPROMCommit();
}