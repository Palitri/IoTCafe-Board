#include "Peripheral.h"

Peripheral::Peripheral() :
	BasePropertiesObject()
{
	this->device = null;
}

Peripheral::Peripheral(IClusterDevice* device) :
	BasePropertiesObject()
{
	this->device = device;
}

Peripheral::~Peripheral()
{
	//this->SetPropertiesCapacity(0);
}

int Peripheral::Load(const void* code)
{
	return 0;
}

void Peripheral::Update()
{
}
