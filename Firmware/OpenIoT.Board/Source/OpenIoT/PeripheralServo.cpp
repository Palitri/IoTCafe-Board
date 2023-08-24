#include "PeripheralServo.h"

#include "Board.h"

PeripheralServo::PeripheralServo(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);

	this->servo = null;
}

PeripheralServo::~PeripheralServo()
{
	if (this->servo != null)
		delete this->servo;
}


int PeripheralServo::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->pValue = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));

	// TODO: Put creation in constructor
	this->servo = new ServoControl(this->pin);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralServo::Update()
{
	this->UpdateProperties();

	if (this->pValue->IsChanged())
		this->servo->WriteDegreece(this->pValue->fValue * 180.0f);
}
