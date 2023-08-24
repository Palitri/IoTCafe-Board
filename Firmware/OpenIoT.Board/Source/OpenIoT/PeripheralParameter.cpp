#include "PeripheralParameter.h"

PeripheralParameter::PeripheralParameter(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);
}

PeripheralParameter::~PeripheralParameter()
{

}


int PeripheralParameter::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->parameter = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));

	return (unsigned int)charCode - (unsigned int)code;
}
