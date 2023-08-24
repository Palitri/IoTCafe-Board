#include "PeripheralInterpolation.h"

#include "Math.h"


PeripheralInterpolation::PeripheralInterpolation(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

PeripheralInterpolation::~PeripheralInterpolation()
{

}


int PeripheralInterpolation::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->input = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->output = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));

	this->inMin = *(float*)charCode;
	charCode += 4;
	this->inDelta = *(float*)charCode - this->inMin;
	charCode += 4;
	this->outMin = *(float*)charCode;
	charCode += 4;
	this->outDelta = *(float*)charCode - this->outMin;
	charCode += 4;
	this->exponentialSlope = *(float*)charCode;
	charCode += 4;
	this->mode = *(unsigned short*)charCode;
	charCode += 2;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralInterpolation::Update()
{
	float factor = this->inDelta == 0.0f ? 0.0f : (this->input->fValue - this->inMin) / this->inDelta;

	switch (this->mode)
	{
	case PeripheralInterpolation::Mode_Linear:
	{
		this->output->SetFloat(this->outMin + this->outDelta * factor);

		break;
	}

	case PeripheralInterpolation::Mode_Exponential:
	{
		this->output->SetFloat(this->outMin + Math::Exponent(factor, this->exponentialSlope - this->outMin) * this->outDelta);

		break;
	}

	case PeripheralInterpolation::Mode_Sine:
	{
		this->output->SetFloat(this->outMin + (Math::Sin((factor - 0.5f) * Math::Pi) * 0.5f + 0.5f) * this->outDelta);

		break;
	}
	}
}
