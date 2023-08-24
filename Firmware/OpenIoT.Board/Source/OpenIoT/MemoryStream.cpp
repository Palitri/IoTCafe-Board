#include "MemoryStream.h"

#include "Board.h"
#include "Mem.h"
#include "Math.h"

MemoryStream::MemoryStream()
{
	this->data = null;
	this->size = 0;
	this->capacity = 0;
	this->position = 0;
}

MemoryStream::~MemoryStream()
{
	this->SetCapacity(0);
}

int MemoryStream::AvailableInputBytes()
{
	return this->size - this->position;
}

int MemoryStream::Read(void* destination, int count)
{
	int length = Math::Min(count, this->AvailableInputBytes());

	Mem::Copy(destination, (char*)this->data + this->position, length);
	this->position += length;

	return length;
}

int MemoryStream::Write(const void* source, int count)
{
	this->EnsureCapacity(this->position + count);
	
	Mem::Copy((char*)this->data + this->position, source, count);
	this->position += count;

	this->size = Math::Max(this->size, this->position);

	return count;
}

int MemoryStream::Seek(int position, int seekMode)
{
	switch (seekMode)
	{
		case SeekMode_Offset:
			this->position += position;
			break;

		case SeekMode_Absolute:
			this->position = position;
			break;
	}

	return position;
}

int MemoryStream::GetPosition()
{
	return this->position;
}

void MemoryStream::Flush()
{

}

void MemoryStream::SetCapacity(int capacity, bool keepData)
{
	if (this->capacity == capacity)
		return;

	this->size = Math::Min(this->size, capacity);
	this->position = Math::Min(this->position, this->size);

	if (keepData)
	{
		void* newData;
		if (capacity > 0)
		{
			newData = new char[capacity];
			Mem::Copy(newData, this->data, this->size);
		}
		else
			newData = null;

		if (this->capacity > 0)
			delete[] this->data;

		this->data = newData;
	}
	else
	{
		if (this->capacity > 0)
			delete[] this->data;

		if (capacity > 0)
			this->data = new char[capacity];
	}

	this->capacity = capacity;
}

void MemoryStream::EnsureCapacity(int capacity)
{
	if (this->capacity >= capacity)
		return;

	int sizeStep = Math::Trim(this->capacity, 16, 64);
	int newCapacity = ((capacity / sizeStep) + 1) * sizeStep;

	this->SetCapacity(newCapacity);
}

void MemoryStream::Clear()
{
	this->position = 0;
	this->size = 0;
}