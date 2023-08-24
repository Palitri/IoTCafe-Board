#include "UltrasonicSensor.h"

#include "Board.h"

const int UltrasonicSensor::triggerPulseMicroseconds = 10;
const float UltrasonicSensor::soundSpeedInDryAirMetersPerSecond = 343.2f;

// maxDitance determines the maximum distance to be measured. 
// Since greater distances require greater measurement time, limiting the maximum distance will improve processing speed when sensor is put before distances larger than the given maximum distance
void UltrasonicSensor::Init(int pinTrigger, int pinEcho, float maxDistance)
{
	this->soundSpeedMetersPerSecond = UltrasonicSensor::soundSpeedInDryAirMetersPerSecond;

	this->pinTrigger = pinTrigger;
	this->pinEcho = pinEcho;

	this->SetMaxDistance(maxDistance);

	Board::SetPinMode(this->pinTrigger, BoardPinMode_DigitalOutput);
	Board::SetPinMode(this->pinEcho, BoardPinMode_DigitalInput);

	Board::DigitalWrite(this->pinTrigger, false);
}

void UltrasonicSensor::SetMaxDistance(float maxDistance)
{
	this->maxTimeoutMicroseconds = (unsigned long)(this->GetDistanceTime(maxDistance) * 2000000.0f);
}

void UltrasonicSensor::Sense()
{
	Board::DigitalWrite(this->pinTrigger, true);
	Board::DelayMicros(UltrasonicSensor::triggerPulseMicroseconds);
	Board::DigitalWrite(this->pinTrigger, false);

	this->echoDurationMicroseconds = Board::AwaitInput(this->pinEcho, true, this->maxTimeoutMicroseconds);
	this->distanceAtLimit = this->echoDurationMicroseconds == 0;
	if (!this->distanceAtLimit)
		this->distance = ((float)this->echoDurationMicroseconds * this->soundSpeedMetersPerSecond) / 2000000.0f;
}

float UltrasonicSensor::GetDistanceTime(float travelDistanceMeters)
{
	return  travelDistanceMeters / this->soundSpeedMetersPerSecond;
}

float UltrasonicSensor::GetTimeDistance(float timeSeconds)
{
	return timeSeconds * this->soundSpeedMetersPerSecond;
}


