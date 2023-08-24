#include "PeripheralTrigger.h"

#include "Math.h"

#include "Log.h"

PeripheralTrigger::PeripheralTrigger(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

PeripheralTrigger::~PeripheralTrigger()
{

}


int PeripheralTrigger::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->input = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->output = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	this->triggerOnRise = *(bool*)charCode++;
	this->triggerOnFall = *(bool*)charCode++;
	this->triggerOnMatch = *(bool*)charCode++;
	this->mode = *(unsigned short*)charCode;
	charCode += 2;
	this->delayTimeMillis = (int)((*(float*)charCode) * 1000.0f);
	charCode += 4;
	this->referenceValue = *(float*)charCode;
	charCode += 4;
	this->referenceThreshold = *(float*)charCode;
	charCode += 4;

	this->lastTriggeredTime = 0;
	this->riseable = false;
	this->fallable = false;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralTrigger::Update()
{
	float value = this->input->fValue;
	float referenceDelta = value - this->referenceValue;

	bool isBelow = referenceDelta <= -this->referenceThreshold;
	bool isAbove = referenceDelta >= this->referenceThreshold;
	bool isContinuous = this->mode == PeripheralTrigger::Mode_Continuous;

	this->riseable |= isBelow;
	this->fallable |= isAbove;
	this->matchable |= isBelow || isAbove;

	bool triggered = false;

	if (this->riseable && referenceDelta >= 0.0f)
	{
		triggered |= this->triggerOnRise;
		this->riseable = isContinuous;
	}

	if (this->fallable && referenceDelta <= 0.0f)
	{
		triggered |= this->triggerOnFall;
		this->fallable = isContinuous;
	}

	if (this->matchable && Math::Abs(referenceDelta) <= this->referenceThreshold * 0.5)
	{
		triggered |= this->triggerOnMatch;
		this->matchable = isContinuous;
	}

	switch (this->mode)
	{
		case PeripheralTrigger::Mode_OneTime:
		case PeripheralTrigger::Mode_Continuous:
		{
			unsigned long time = Board::TimeMillis();
			if (triggered)
				this->lastTriggeredTime = time;

			bool active = time - this->lastTriggeredTime <= this->delayTimeMillis;
			this->output->SetFloat(active ? 1.0f : 0.0f);

			break;
		}

		case PeripheralTrigger::Mode_Switch:
		{
			if (triggered)
				this->output->SetFloat(this->output->fValue != 0.0f ? 0.0f : 1.0f);

			break;
		}
	}
}
