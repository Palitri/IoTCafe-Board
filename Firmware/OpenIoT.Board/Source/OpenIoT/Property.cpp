#include "Property.h"

#include "Mem.h"
#include "Board.h"

#include "Strings.h"

unsigned char Property::typeSizes[5] = { 0, 0, 4, 4, 1 };

void Property::Init(unsigned char semantic, PropertyType type, unsigned char flags)
{
	this->semantic = semantic;
	this->type = type;
	this->flags = flags;

	this->data = null;
}

void Property::InitWithValue(void** data, PropertyType type, unsigned char flags)
{
	unsigned char semantic = *(unsigned char*)*data;
	*(unsigned char**)data += 1;

	this->Init(semantic, type, flags);

	*(unsigned char**)data += this->ReadValueFromStream(*data);
}


Property::Property()
{
}

Property::~Property()
{
	this->SetDataSize(0);
}

Property::Property(unsigned char semantic, int value, unsigned char flags)
{
	this->Init(semantic, PropertyType_Integer, flags);
	this->iValue = value;
}

Property::Property(unsigned char semantic, float value, unsigned char flags)
{
	this->Init(semantic, PropertyType_Float, flags);
	this->fValue = value;
}

// The ProprtyType param is there, just to distinguish from the int overload, because the compiler interprets this constructor(bool) to the int one
Property::Property(unsigned char semantic, bool value, PropertyType type, unsigned char flags)
{
	this->Init(semantic, PropertyType_Bool, flags);
	this->bValue = value;
}

Property::Property(Property* param, unsigned char semantic, unsigned char flags)
{
	this->Init(semantic, param->type, flags);
	this->iValue = param->iValue;
}



Property::Property(void** data, PropertyType type, unsigned char flags)
{
	this->InitWithValue(data, type, flags);
}

void Property::SetBool(bool value)
{
	this->SetValue(&value);
}

void Property::SetInt(int value)
{
	this->SetValue(&value);
}

void Property::SetStringData(const char* str)
{
	this->SetDataValue((void*)str, Strings::Length((char*)str) + 1);
}

char* Property::GetStringData()
{
	return (char*)this->data;
}

void Property::SetFloat(float value)
{
	this->SetValue(&value);
}

bool Property::IsReadable()
{
	return (this->flags & PropertyFlag_Read) != 0;
}

bool Property::IsWritable()
{
	return (this->flags & PropertyFlag_Write) != 0;
}

bool Property::IsEEPRom()
{
	return (this->flags & PropertyFlag_EEPRom) != 0;
}

bool Property::IsSubscribed()
{
	return (this->flags & PropertyFlag_Subscribed) != 0;
}

int Property::Size()
{
	if (this->type == PropertyType_Data)
		return *((unsigned char*)this->data - 1);
	else
		return Property::typeSizes[(int)this->type];
}

void Property::SetValue(void* value)
{
	void* address = this->type == PropertyType_Data ? this->data : &this->data;

	int size = this->Size();
	bool changed = Mem::Compare(value, address, size) != 0;
	if (changed)
		Mem::Copy(address, value, size);
	this->isChanged |= changed;
}

bool Property::SetDataSize(int size)
{
	if (this->type != PropertyType_Data)
		return false;

	if (this->data != null)
	{
		if (size == this->Size())
			return false;

		Mem::Free((unsigned char*)this->data - 1);
		this->data = null;
	}

	if (size > 0)
	{
		this->data = Mem::Get(size + 1);
		*(unsigned char*)this->data = size;
		this->data = (unsigned char*)this->data + 1;
		this->isChanged = true;
	}

	return true;
}

void Property::SetDataValue(void* value, int size)
{
	this->SetDataSize(size);
	this->SetValue(value);
}

void Property::GetValue(void* value)
{
	void* address = this->type == PropertyType_Data ? this->data : &this->data;

	Mem::Copy(value, address, this->Size());
}

int Property::WriteValueToStream(void* value)
{
	int length = this->Size();

	if (this->type == PropertyType_Data)
	{
		Mem::Copy(value, &length, 1);
		length++;
		value = (unsigned char*)value + 1;
	}

	this->GetValue(value);

	return length;
}

int Property::ReadValueFromStream(void* value)
{
	int length = 0;

	if (type == PropertyType_Data)
	{
		this->SetDataSize(*(unsigned char*)value);
		value = (unsigned char*)value + 1;
		length++;
	}

	this->SetValue(value);
	length += this->Size();

	return length;
}

bool Property::IsChanged()
{
	return this->isChanged;
}

void Property::ResetChanged()
{
	this->isChanged = false;
}

void Property::Update()
{

}

int Property::ToString(char* dest, bool nullTerminate)
{
	int result;

	switch (this->type)
	{
		case PropertyType_Integer:
			return Strings::ToDec(this->iValue, dest, nullTerminate);

		case PropertyType_Float:
			return Strings::ToFloat(this->fValue, dest, nullTerminate);

		case PropertyType_Bool:
			return Strings::ToBool(this->bValue, dest, true, nullTerminate);

		case PropertyType_Data:
			return Strings::ToHex(this->data, dest, this->Size(), nullTerminate);
	}

	return 0;
}
