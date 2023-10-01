//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralAnimation.h"

#include "Board.h"
#include "Math.h"

PeripheralAnimation::PeripheralAnimation(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(5);
}

PeripheralAnimation::~PeripheralAnimation()
{

}


int PeripheralAnimation::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->active = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->start = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->length = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->speed = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->output = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	this->forthAndBack = (bool)*charCode++;
	this->loop = (bool)*charCode++;

	this->lastUpdateTime = Board::TimeMillis();
	this->back = false;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralAnimation::Update()
{
	unsigned long time = Board::TimeMillis();
	float deltaTime = (float)(time - this->lastUpdateTime) / 1000.0f;
	this->lastUpdateTime = time;

	float start = this->start->fValue;

	if (this->active->bValue)
	{
		float value = this->output->fValue;

		if (back)
		{
			value -= this->speed->fValue * deltaTime;
			float undershoot = start - value;
			if (undershoot >= 0.0f)
			{
				if (this->loop)
				{
					value = undershoot;
				}
				else
				{
					value = start;
					this->active->SetBool(false);
				}

				this->back = false;
			}
		}
		else
		{
			float end = start + this->length->fValue;

			value += this->speed->fValue * deltaTime;
			float overshoot = value - end;
			if (overshoot >= 0.0f)
			{
				if (this->forthAndBack)
				{
					value = end - overshoot;
					this->back = true;
				}
				else
				{
					if (this->loop)
					{
						value = overshoot;
					}
					else
					{
						value = end;
						this->active->SetBool(false);
					}
				}
			}
		}

		this->output->SetFloat(value);
	}
}
