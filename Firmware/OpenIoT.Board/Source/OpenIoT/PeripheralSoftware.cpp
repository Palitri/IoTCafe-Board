#include "PeripheralSoftware.h"

#include "BoardPinProperty.h"

#include "BitUtils.h"

PeripheralSoftware::PeripheralSoftware(IClusterDevice* device) :
	Peripheral(device)
{
}

PeripheralSoftware::~PeripheralSoftware()
{

}


int PeripheralSoftware::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	//int type;
	//charCode += BitUtils::Read7BitEncodedBytes(charCode, type);

	unsigned char propertiesCount = *charCode++;

	this->SetPropertiesCapacity(propertiesCount);

	for (int i = 0; i < propertiesCount; i++)
	{
		PropertyType type = (PropertyType)*charCode++;
		this->AddProperty(new Property((void**)&charCode, type));
	}

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralSoftware::Update()
{
	this->UpdateProperties();
}
