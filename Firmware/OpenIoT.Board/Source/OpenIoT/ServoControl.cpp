#include "ServoControl.h"

#include "Board.h"

ServoControl::ServoControl(int pin)
{
	this->pin = pin;

	this->servo = Board::ServoBegin(pin);
}

void ServoControl::WriteMicroseconds(int microseconds)
{
	Board::ServoWrite(this->servo, microseconds);
}

void ServoControl::WriteDegreece(float degreece)
{
	//this->servo->write(degreece);
}

