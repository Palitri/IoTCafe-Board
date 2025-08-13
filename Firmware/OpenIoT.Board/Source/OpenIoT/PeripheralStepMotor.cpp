//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralStepMotor.h"

#include "Board.h"

#include "Log.h"

#include "Math.h"

PeripheralStepMotor::PeripheralStepMotor(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(5);

	this->motor = null;
}

PeripheralStepMotor::~PeripheralStepMotor()
{
	if (this->motor != null)
		delete this->motor;

	for (int i = 0; i < this->asyncDriver.deviceChannels.count; i++)
	{
		if (this->asyncDriver.deviceChannels[i]->deviceDriver != null)
			delete this->asyncDriver.deviceChannels[i]->deviceDriver;

		if (this->asyncDriver.deviceChannels[i]->unitMapper != null)
			delete this->asyncDriver.deviceChannels[i]->unitMapper;

		delete this->asyncDriver.deviceChannels[i];
	}
}


int PeripheralStepMotor::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->pTurns = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->pTurnsPerSec = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->pTurn = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->pAccelerationTime = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->pDecelerationTime = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));

	unsigned char pinStep = *charCode++;
	unsigned char pinDir = *charCode++;
	this->stepsPerTurn = *(float*)charCode;
	charCode += 4;

	this->motor = new StepMotorDriver(pinStep, pinDir);
	this->driver = this->motor;

	this->oldTurn = this->pTurn->bValue;
	this->oldTurns = this->pTurns->fValue;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralStepMotor::Update()
{
	// Bool Turn
	if (this->pTurn->bValue != this->oldTurn)
	{
		if (this->pTurn->bValue)
		{
			this->stepsRemaining = 0;
			this->motor->StartRotation(this->pTurnsPerSec->fValue * this->stepsPerTurn);
		}
		else
			this->motor->StopRotation();
		
		this->oldTurn = this->pTurn->bValue;
	}

	// Float Turns
	if (this->pTurns->fValue != this->oldTurns)
	{
		if (this->pTurns->fValue != 0)
		{
			this->stepsRemaining = (int)(this->pTurns->fValue * this->stepsPerTurn);
			this->motor->StartRotation(this->pTurnsPerSec->fValue * this->stepsPerTurn);
		}
		else
			this->motor->StopRotation();
		
		this->oldTurns = this->pTurns->fValue;
	}

	// Rotate
	if (this->pTurn->bValue)
	{
		this->motor->SetRotationSpeed(this->pTurnsPerSec->fValue * this->stepsPerTurn);
		this->motor->RotationRoutines();
	}
	else if (this->pTurns->fValue != 0)
	{
		this->motor->SetRotationSpeed(Math::Sign(this->stepsRemaining) * this->pTurnsPerSec->fValue * this->stepsPerTurn);
		this->stepsRemaining -= this->motor->RotationRoutines();
		this->pTurns->SetFloat(this->stepsRemaining / this->stepsPerTurn);
	}
}
