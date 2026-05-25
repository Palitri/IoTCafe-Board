//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Thermistor.h"

#include "Board.h"
#include "Math.h"

Thermistor::Thermistor(int pin, float betaCoefficient, float referenceTempKelvin, float referenceResistance)
{
	this->pin = pin;
	this->betaCoefficient = betaCoefficient;
	this->referenceTempKelvin = referenceTempKelvin;
	this->referenceResistance = referenceResistance;

	Board::SetPinMode(this->pin, BoardPinMode_AnalogInput);
}

Thermistor::~Thermistor()
{

}

float Thermistor::ReadTemperature()
{
	float resistance = Board::AnalogRead(this->pin) * this->referenceResistance;
	return Thermistor::CalculateTemperature(resistance, this->referenceResistance, this->referenceTempKelvin, this->betaCoefficient);
}

float Thermistor::CalculateTemperature(float resistance, float referenceResistance, float referenceTempKelvin, float betaCoefficient)
{
	return 1.0f / (1.0f / referenceTempKelvin + (1.0f / betaCoefficient) * Math::Ln(resistance / referenceResistance));
}

float Thermistor::CalculateBetaCoefficient(float resistance1, float temp1, float resistance2, float temp2)
{
	return Math::Ln(resistance1 / resistance2) / (1.0f / temp1 - 1.0f / temp2);
}

float Thermistor::CalculateResistance(float tempKelvin, float referenceResistance, float referenceTempKelvin, float betaCoefficient)
{
	return referenceResistance * Math::Power(Math::e, betaCoefficient * (1.0f / tempKelvin - 1.0f / referenceTempKelvin));
}
