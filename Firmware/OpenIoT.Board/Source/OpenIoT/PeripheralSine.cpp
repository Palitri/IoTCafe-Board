#include "PeripheralSine.h"

#include "Math.h"

PeripheralSine::PeripheralSine(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

PeripheralSine::~PeripheralSine()
{

}


int PeripheralSine::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->input = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->output = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	this->phaseStart = *(float*)charCode;
	charCode += 4;
	this->phaseDelta = *(float*)charCode - this->phaseStart;
	charCode += 4;
	this->amplitude = *(float*)charCode;
	charCode += 4;
	this->offset = *(float*)charCode;
	charCode += 4;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralSine::Update()
{
	this->output->SetFloat(this->offset + Math::Sin((this->phaseStart + this->input->fValue * this->phaseDelta) * Math::Pi2) * this->amplitude);
}
