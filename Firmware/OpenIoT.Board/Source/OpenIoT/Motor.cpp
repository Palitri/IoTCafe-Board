//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Motor.h"

#include "Math.h"

Motor::Motor()
{
	this->limitOutputValue = false;
	this->threshold = 0.0f;
	this->minRiseTime = 0.0f;
	this->minFallTime = 0.0f;
	this->traction = 0.0f;
}

Motor::~Motor()
{
}

void Motor::SetTraction(float traction)
{
	this->traction = traction;

	if (this->limitOutputValue)
		this->traction = Math::Trim(this->traction, 0.0f, 1.0f);

	float motorTraction = (1.0f + this->threshold * 2.0f) * traction - this->threshold;

	this->SetMotorTraction(motorTraction);
}

void Motor::SetTraction(float traction, float deltaTime)
{
	float deltaTraction = traction - this->traction;
	if (deltaTraction > 0.0f)
	{
		if (this->minRiseTime > 0.0f)
			deltaTraction = Math::Min(deltaTraction, deltaTime / this->minRiseTime);
	}
	else
	{
		if (this->minFallTime > 0.0f)
			deltaTraction = Math::Max(deltaTraction, -deltaTime / this->minFallTime);
	}

	this->SetTraction(this->traction + deltaTraction);
}

