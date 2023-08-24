#include "PeripheralTransition.h"

#include "Board.h"
#include "Math.h"


PeripheralTransition::PeripheralTransition(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

PeripheralTransition::~PeripheralTransition()
{

}


int PeripheralTransition::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->input = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->output = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	this->speed = *(float*)charCode;
	charCode += 4;
	this->exponential = *(bool*)charCode;
	charCode += 1;

	this->lastUpdateTime = 0;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralTransition::Update()
{
	unsigned long time = Board::TimeMillis();
	float deltaTime = (float)(time - this->lastUpdateTime) / 1000.0f;
	this->lastUpdateTime = time;

	float delta = this->input->fValue - this->output->fValue;

	if (this->exponential)
	{
		this->output->SetFloat(this->output->fValue + delta * Math::Trim(this->speed * deltaTime, 0.0f, 1.0f));
	}
	else
	{
		if (delta >= 0.0f)
			this->output->SetFloat(this->output->fValue + Math::Trim(this->speed * deltaTime, 0.0f, delta));
		else
			this->output->SetFloat(this->output->fValue - Math::Trim(this->speed * deltaTime, 0.0f, -delta));
	}

}
