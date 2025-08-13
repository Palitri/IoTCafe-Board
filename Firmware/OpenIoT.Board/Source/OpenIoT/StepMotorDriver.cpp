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

StepMotorDriver::StepMotorDriver(int pinStep, int pinDir)
{
	this->pinStep = pinStep;
	this->pinDir = pinDir;

	this->rotatingDirection = StepMotorDriver::DirectionForward;

	Board::SetPinMode(this->pinDir, BoardPinMode_DigitalOutput);
	Board::SetPinMode(this->pinStep, BoardPinMode_DigitalOutput);

	Board::DigitalWrite(this->pinDir, this->rotatingDirection);
	Board::DigitalWrite(this->pinStep, false);

	this->drivePulsesTotal = 0;
	this->drivePulsesMade = 0;
	this->driveStepPinValue = false;

	this->isRotating = false;
}

StepMotorDriver::~StepMotorDriver()
{
}


void StepMotorDriver::SetRotationSpeed(float stepsPerSecond)
{
	this->stepsPerSecond = stepsPerSecond;
	this->pulseIntervalMicros = Math::Abs((int)(500000.0f / stepsPerSecond));

	this->SetupDirection();
}

void StepMotorDriver::StartRotation(float stepsPerSecond)
{
	this->isRotating = true;
	this->lastPulseTime = Board::TimeMicros();

	this->SetRotationSpeed(stepsPerSecond);
}

void StepMotorDriver::StopRotation()
{
	this->isRotating = false;
}


void StepMotorDriver::SetupDirection()
{
	this->rotationSign = this->stepsPerSecond > 0.0f ? 1 : (this->stepsPerSecond < 0.0f ? -1 : 0);
	bool rotatingDirection = this->rotationSign < 0;
	if (rotatingDirection != this->rotatingDirection)
	{
		Board::DigitalWrite(this->pinDir, rotatingDirection);
		this->rotatingDirection = rotatingDirection;
	}
}

bool StepMotorDriver::IsRotating()
{
	return this->isRotating;
}

int StepMotorDriver::RotationRoutines()
{
	if (!this->isRotating)
		return 0;

	unsigned long time = Board::TimeMicros();
	if (time - this->lastPulseTime >= this->pulseIntervalMicros)
	{
		this->driveStepPinValue = !this->driveStepPinValue;
		Board::DigitalWrite(this->pinStep, this->driveStepPinValue);

		this->lastPulseTime += this->pulseIntervalMicros;

		return this->driveStepPinValue * this->rotationSign;
	}

	return 0;
}



/// IDeviceDriver methods

void StepMotorDriver::Begin(float origin, float vector)
{
	float pulsesRemainder = this->drivePulsesTotal - this->drivePulsesMade;
	this->drivePulsesTotal = 2.0f * vector + pulsesRemainder;
	this->drivePulsesMade = 0;
}

void StepMotorDriver::Drive(float phase)
{
	float targetPulse = phase * this->drivePulsesTotal;
	int targetPulseDiscreet = (int)Math::Round(targetPulse);
	int pulsesToMake = targetPulseDiscreet - this->drivePulsesMade;
	this->drivePulsesMade = targetPulseDiscreet;

	if (pulsesToMake == 0)
		return;

	unsigned char direction;
	if (pulsesToMake > 0)
	{
		direction = StepMotorDriver::DirectionForward;
	}
	else
	{
		direction = StepMotorDriver::DirectionBackward;
		pulsesToMake = -pulsesToMake;
	}

	if (direction != this->rotatingDirection)
	{
		this->rotatingDirection = direction;
		Board::DigitalWrite(this->pinDir, this->rotatingDirection);
	}

	do
	{
		this->driveStepPinValue = !this->driveStepPinValue;
		Board::DigitalWrite(this->pinStep, this->driveStepPinValue);

		pulsesToMake--;
	} while (pulsesToMake > 0);
}
