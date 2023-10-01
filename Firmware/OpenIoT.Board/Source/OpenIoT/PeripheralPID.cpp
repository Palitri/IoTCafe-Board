//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralPID.h"

#include "Board.h"
#include "Math.h"

PeripheralPID::PeripheralPID(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

PeripheralPID::~PeripheralPID()
{

}


int PeripheralPID::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->input = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->output = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->target = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->p = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->i = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->d = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));

	this->errorLast = 0.0f;
	this->errorIntegral = 0.0f;

	this->lastUpdateTime = 0.0f;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralPID::Update()
{
	unsigned long time = Board::TimeMillis();
	float deltaTime = (float)(time - this->lastUpdateTime) / 1000.0f;

	this->output->SetFloat(this->Compute(this->target->fValue - this->input->fValue, time));
}

float PeripheralPID::Compute(float error, float time)
{
	this->errorIntegral += error * time;
	float errorDerivative = (error - this->errorLast) / time;

	this->errorLast = error;

	return this->p->fValue * error + this->i->fValue * this->errorIntegral + this->d->fValue * errorDerivative;
}
