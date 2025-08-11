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

	this->direction = StepMotorDriver::DirectionForward;

	Board::SetPinMode(this->pinDir, BoardPinMode_DigitalOutput);
	Board::SetPinMode(this->pinStep, BoardPinMode_DigitalOutput);

	Board::DigitalWrite(this->pinDir, this->direction);
	Board::DigitalWrite(this->pinStep, false);

	this->drivePulsesTotal = 0;
	this->drivePulsesMade = 0;
	this->asyncStepState = false;
}

StepMotorDriver::~StepMotorDriver()
{
}

void StepMotorDriver::SetDirection(unsigned char direction)
{
	if (direction != this->direction)
	{
		this->direction = direction;
		Board::DigitalWrite(this->pinDir, this->direction);
	}
}

void StepMotorDriver::Step()
{
	Board::DigitalWrite(this->pinStep, true);
	Board::DigitalWrite(this->pinStep, false);
}

void StepMotorDriver::Step(unsigned char direction)
{
	if (direction != this->direction)
	{
		this->direction = direction;
		Board::DigitalWrite(this->pinDir, this->direction);
	}

	Board::DigitalWrite(this->pinStep, true);
	Board::DigitalWrite(this->pinStep, false);
}

void StepMotorDriver::RotateBySpeed(int steps, float stepsPerSecond)
{
	if (!this->SetDirectionAndNormalizeParameters(steps, stepsPerSecond))
		return;

	// TODO: Consider using Bressenham for precision, but must work with stepsPerSecond < 0
	int intervalMicros = (int)(500000.0f / stepsPerSecond);
	for (int i = 0; i < steps; i++)
	{
		Board::DigitalWrite(this->pinStep, true);
		Board::DelayMicros(intervalMicros);
		Board::DigitalWrite(this->pinStep, false);
		Board::DelayMicros(intervalMicros);
	}
}

void StepMotorDriver::RotateByTime(int steps, float seconds)
{
	if (seconds == 0)
		return;

	this->RotateBySpeed(steps, (float)steps / seconds);
}

void StepMotorDriver::BeginAsyncRotateBySpeed(int steps, float stepsPerSecond, float minStepInterval)
{
	if (!this->SetDirectionAndNormalizeParameters(steps, stepsPerSecond))
		return;

	this->asyncIntervalMicros = (int)(500000.0f / stepsPerSecond);
	this->asyncPulsesRemaining = steps * 2;
	this->asyncTime = Board::TimeMicros();
	this->asyncStepState = false;

	Board::DigitalWrite(this->pinStep, this->asyncStepState);
}

void StepMotorDriver::RotateAsync()
{
	if (this->asyncPulsesRemaining > 0)
	{
		unsigned long time = Board::TimeMicros();
		if (time - this->asyncTime >= this->asyncIntervalMicros)
		{
			this->asyncStepState = !this->asyncStepState;
			Board::DigitalWrite(this->pinStep, this->asyncStepState);

			this->asyncPulsesRemaining--;
			this->asyncTime += this->asyncIntervalMicros;
		}
	}
}

void StepMotorDriver::SetRotateAsyncSpeed(float stepsPerSecond)
{
	this->asyncIntervalMicros = (int)(500000.0f / stepsPerSecond);
}

int StepMotorDriver::GetRemainingAsyncSteps()
{
	return (this->direction == StepMotorDriver::DirectionForward ? this->asyncPulsesRemaining : -this->asyncPulsesRemaining) / 2;
}

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

	if (direction != this->direction)
	{
		this->direction = direction;
		Board::DigitalWrite(this->pinDir, this->direction);
	}

	do
	{
		this->asyncStepState = !this->asyncStepState;
		Board::DigitalWrite(this->pinStep, this->asyncStepState);

		pulsesToMake--;
	} while (pulsesToMake > 0);
}

bool StepMotorDriver::SetDirectionAndNormalizeParameters(int& steps, float& rate)
{
	float work = (float)steps * rate;

	if (work == 0)
		return false;

	unsigned char direction = work >= 0 ? StepMotorDriver::DirectionForward : StepMotorDriver::DirectionBackward;
	steps = Math::Abs(steps);
	rate = Math::Abs(rate);

	if (direction != this->direction)
	{
		this->direction = direction;
		Board::DigitalWrite(this->pinDir, this->direction);
	}

	return true;
}
