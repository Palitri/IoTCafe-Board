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
#include "SineUnitMapper.h"

PeripheralStepMotor::PeripheralStepMotor(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(5);

	this->motor = null;
	this->asyncStepsRemaining = 0;
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

	//
	// Log::println("Creating test setup..");
	// this->asyncDriver.deviceChannels.SetSize(2);
	// this->asyncDriver.deviceChannels[0] = new DeviceChannel();
	// this->asyncDriver.deviceChannels[1] = new DeviceChannel();
	// this->asyncDriver.deviceChannels[0]->deviceDriver = new StepMotorDriver(pinStep, pinDir);
	// this->asyncDriver.deviceChannels[1]->deviceDriver = new StepMotorDriver(4, 2);
	// this->asyncDriver.deviceChannels[0]->unitMapper = new SineUnitMapper();
	// this->asyncDriver.deviceChannels[1]->unitMapper = new SineUnitMapper();
	// ((SineUnitMapper*)(this->asyncDriver.deviceChannels[0]->unitMapper))->Setup(0.5f, 1.0f, 0.0f, 1.0f);
	// ((SineUnitMapper*)(this->asyncDriver.deviceChannels[1]->unitMapper))->Setup(0.5f, 1.0f, 0.25f, 1.25f);
	// Log::println("Test setup created.");
	//

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralStepMotor::Update()
{
	// if ((this->pTurns->fValue != 0.0) && (this->asyncDriver.deviceChannels.count == 2))
	// {
	// 	Log::println("Starting test..");
	// 	this->asyncDriver.deviceChannels[0]->deviceDriver->Begin(0, this->stepsPerFullTurn * this->pTurns->fValue);
	// 	this->asyncDriver.deviceChannels[1]->deviceDriver->Begin(0, this->stepsPerFullTurn * this->pTurns->fValue);

	// 	this->asyncDriver.Drive(3.0);

	// 	this->pTurns->SetFloat(0);
	// 	this->UpdateProperties();

	// 	Log::println("Test done..");
	// }

	if (this->asyncStepsRemaining != 0)
	{
		int stepsPerSec = (int)(this->pTurnsPerSec->fValue * this->stepsPerTurn);
		this->motor->SetRotateAsyncSpeed(stepsPerSec);
		this->motor->RotateAsync();

		this->asyncStepsRemaining = this->motor->GetRemainingAsyncSteps();

		this->pTurns->SetFloat((float)this->asyncStepsRemaining / (float)this->stepsPerTurn);

		this->UpdateProperties();
	}
	else if (this->pTurns->fValue != 0.0)
	{
		int steps = (int)(this->pTurns->fValue * this->stepsPerTurn);
		int stepsPerSec = (int)(this->pTurnsPerSec->fValue * this->stepsPerTurn);

		this->motor->BeginAsyncRotateBySpeed(steps, stepsPerSec);
		
		this->asyncStepsRemaining = this->motor->GetRemainingAsyncSteps();
	}
}
