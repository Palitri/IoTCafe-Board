//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "StepMotorDriver.h"

#include "Board.h"

#include "Math.h"

StepMotorDriver::StepMotorDriver(int pinStep, int pinDir, float distanceUnitsPerTurn, int fullStepsPerTurn)
{
	this->pinStep = pinStep;
	this->pinDir = pinDir;

	this->distanceUnitsPerTurn = distanceUnitsPerTurn;
	this->fullStepsPerTurn = fullStepsPerTurn;

	this->stepState = false;
	this->forward = true;

	this->vectorStepsTotal = 0.0f;
	this->currentStep = 0;

	Board::SetPinMode(this->pinStep, BoardPinMode_DigitalOutput);
	Board::SetPinMode(this->pinDir, BoardPinMode_DigitalOutput);

	Board::DigitalWrite(this->pinStep, this->stepState);
	Board::DigitalWrite(this->pinDir, this->forward);

	this->SetStepMultiplier(1);
}

StepMotorDriver::~StepMotorDriver()
{
}

void StepMotorDriver::Begin(float origin, float vector)
{
	float remainder = this->vectorStepsTotal - this->currentStep;
	this->vectorStepsTotal = 2.0f * vector * this->stepsPerUnit + remainder;
	this->currentStep = 0;
}

void StepMotorDriver::Drive(float phase)
{
	float targetStep = phase * this->vectorStepsTotal;
	int targetStepDiscreet = (int)Math::Round(targetStep);
	int motorSteps = targetStepDiscreet - this->currentStep;
	this->currentStep = targetStepDiscreet;

	bool forward = motorSteps >= 0;
	if (this->forward != forward)
	{
		this->forward = forward;
		Board::DigitalWrite(this->pinDir, forward);
	}

	if (!forward)
		motorSteps = -motorSteps;

	while (motorSteps > 0)
	{
		this->stepState = !this->stepState;
		Board::DigitalWrite(this->pinStep, this->stepState);

		motorSteps--;
	}
}

float StepMotorDriver::CalculateStepsPerUnit()
{
	return (float)(this->stepMultiplier * this->fullStepsPerTurn) / this->distanceUnitsPerTurn;
};

float StepMotorDriver::CalculateStepIntervalForSpeed(float speedUnitsPerSecond)
{
	return 1000000.0f / (this->CalculateStepsPerUnit() * speedUnitsPerSecond);
}

void StepMotorDriver::SetStepMultiplier(int stepMultiplier)
{
	this->stepMultiplier = stepMultiplier;
	this->stepsPerUnit = this->CalculateStepsPerUnit();
}

void StepMotorDriver::SetDirection(bool forward)
{
	this->forward = forward;
	Board::DigitalWrite(this->pinDir, !this->forward);
}

void StepMotorDriver::Pulse(bool forward, bool pulseState)
{
	this->forward = forward;
	Board::DigitalWrite(this->pinDir, !this->forward);

	this->stepState = pulseState;
	Board::DigitalWrite(this->pinStep, this->stepState);
}

void StepMotorDriver::Pulse(bool forward)
{
	this->forward = forward;
	Board::DigitalWrite(this->pinDir, !this->forward);

	this->stepState = !this->stepState;
	Board::DigitalWrite(this->pinStep, this->stepState);
}

void StepMotorDriver::Pulse()
{
	this->stepState = !this->stepState;
	Board::DigitalWrite(this->pinStep, this->stepState);
}


