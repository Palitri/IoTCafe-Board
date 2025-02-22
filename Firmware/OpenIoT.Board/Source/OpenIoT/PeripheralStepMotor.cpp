//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralStepMotor.h"

#include "Board.h"

PeripheralStepMotor::PeripheralStepMotor(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);

	this->motor = null;
	this->asyncStepsRemaining = 0;
}

PeripheralStepMotor::~PeripheralStepMotor()
{
	if (this->motor != null)
		delete this->motor;
}


int PeripheralStepMotor::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->pTurns = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->pTurnsPerSec = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));


	unsigned char pinStep = *charCode++;
	unsigned char pinDir = *charCode++;
	this->stepsPerFullTurn = *(float*)charCode;
	charCode += 4;

	this->motor = new StepMotorDriver(pinStep, pinDir);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralStepMotor::Update()
{
	if (this->asyncStepsRemaining != 0)
	{
		this->motor->RotateAsync();

		this->asyncStepsRemaining = this->motor->GetRemainingAsyncSteps();

		this->pTurns->SetFloat((float)this->asyncStepsRemaining / (float)this->stepsPerFullTurn);

		this->UpdateProperties();
	}
	else if (this->pTurns->fValue != 0.0)
	{
		int steps = (int)(this->pTurns->fValue * this->stepsPerFullTurn);
		int stepsPerSec = (int)(this->pTurnsPerSec->fValue * this->stepsPerFullTurn);

		this->motor->BeginAsyncRotateBySpeed(steps, stepsPerSec);
		
		this->asyncStepsRemaining = this->motor->GetRemainingAsyncSteps();
	}
}
