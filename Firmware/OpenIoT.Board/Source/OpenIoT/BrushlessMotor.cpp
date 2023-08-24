#include "BrushlessMotor.h"

BrushlessMotor::BrushlessMotor(int motorPin, int minValue, int maxValue)
	: Motor()
{
	this->limitOutputValue = true;
	this->threshold = 0.05f;
	this->minRiseTime = 0.5f;
	this->minFallTime = 0.5f;

	this->motorPin = motorPin;
	this->minValue = minValue;
	this->maxValue = maxValue;

	this->servo = new ServoControl(this->motorPin);

	this->SetMotorTraction(0.0f);
}

BrushlessMotor::~BrushlessMotor()
{
	delete this->servo;
}

void BrushlessMotor::SetMotorTraction(float motorTraction)
{
	int servoValue = this->minValue + (int)((float)(this->maxValue - this->minValue) * motorTraction);
	this->servo->WriteMicroseconds(servoValue);
}
