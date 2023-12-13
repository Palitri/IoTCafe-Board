//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "DHTSensor.h"

#include "Board.h"

typedef float (DHTSensorGetDataValueHandler)(const char* data);

const int DHTSensor::WakeupSignalInterval_DHT11 = 18000;
const int DHTSensor::WakeupSignalInterval_DHT22 = 1000;

const float DHTSensor::MinUpdateInterval_DHT11 = 2.0f;
const float DHTSensor::MinUpdateInterval_DHT22 = 1.0f;

const int DHTSensor::InitializationInterval = 1000000;


DHTSensor::DHTSensor(int pin, char model)
{
	this->model = model;
	this->pin = pin;

	switch (this->model)
	{
		case DHTSensor::Model_DHT22:
		{
			this->wakeupSignalInterval = DHTSensor::WakeupSignalInterval_DHT22;
			this->getTemperatureHandler = (void*)DHTSensor::GetTemperatureDHT22;
			this->getHumidityHandler = (void*)DHTSensor::GetHumidityDHT22;
			this->minUpdateInterval = DHTSensor::MinUpdateInterval_DHT22;

			break;
		}

		case DHTSensor::Model_DHT11:
		default:
		{
			this->wakeupSignalInterval = DHTSensor::WakeupSignalInterval_DHT11;
			this->getTemperatureHandler = (void*)DHTSensor::GetTemperatureDHT11;
			this->getHumidityHandler = (void*)DHTSensor::GetHumidityDHT11;
			this->minUpdateInterval = DHTSensor::MinUpdateInterval_DHT11;

			break;
		}
	}

	this->temperature = 0.0f;
	this->humidity = 0.0f;

	// Specs define the minumum interval and ask for "at least" that, so add extra 12.5% time, just in case
	this->wakeupSignalInterval += this->wakeupSignalInterval >> 3;

	this->changing = false;

	this->bitIndex = 0;
	this->time = Board::TimeMicros();
	this->state = DHTSensor::State_Initializing;
}

DHTSensor::~DHTSensor()
{
}

//	0. Initialization state: 1s after power supplied. Do nothing while in this state
//	1. Default state: Line is HIGH
//	2. Start
//		2.1. Set line LOW for at least 18000us for DHT11, 1000 for DHT22
//		2.2. Set line HIGH and wait for response for 20-40us
//	3. Start Response
//		3.1. Get LOW 80us
//		3.2. Get HIGH 80us
//	4. Data bits: 40 bit iterations. Bits: [8] Temp.whole, [8] Temp.fract, [8] Humid.whole, [8] Humid.fract, [8] Checksum = Sum of previous 4 bytes
//		4.1. Data bit: Get LOW 50us
//		4.2. Data bit: Get HIGH
//			a) Bit 0: 26-28us
//			b) Bit 1: 70us
//	5. End
//		5.1. Get LOW 50us
//		5.2. Get HIGH and back to Default state
void DHTSensor::RequestUpdate()
{
	if (this->state == DHTSensor::State_Initializing)
	{
		unsigned long time = Board::TimeMicros();
		long deltaTime = time - this->time;
		if (deltaTime < DHTSensor::InitializationInterval)
			return;
	}


	this->state = DHTSensor::State_Default;

	Board::SetPinMode(this->pin, BoardPinMode_DigitalOutput);
	Board::DigitalWrite(this->pin, false);

	Board::DelayMicros(this->wakeupSignalInterval);// 2.1. >=18000us(DHT11), >=1000us(DHT22)
	Board::DigitalWrite(this->pin, true);

	Board::SetPinMode(this->pin, BoardPinMode_DigitalInput_Pullup); // 2.2. Set High and wait 20-40us for reponse: 80us Low, 80us Down, Data

	this->time = Board::TimeMicros();
	this->state = DHTSensor::State_WakingUp;
}

bool DHTSensor::OnChange()
{
	if (this->changing)
		return false;

	if (this->state < DHTSensor::State_Default)
		return false;
	
	this->changing = true;
	
	unsigned long time = Board::TimeMicros();
	long deltaTime = time - this->time;
	this->time = time;

	bool isHigh = Board::DigitalRead(this->pin);
	bool isLow = !isHigh;

	bool result = false;

	switch (this->state)
	{
		case DHTSensor::State_WakingUp:
		{
			if (isLow)
			{
				this->bitIndex = 0;
				this->state = DHTSensor::State_Reading; // From 3.2 to 4.1
			}
			break;
		}

		// Consider only writing timings in uchar[40] array and processing it out of the interrupt for performance.
		case DHTSensor::State_Reading:
		{
			if (isLow) // From 4.2 back to 4.1 or to 5.1
			{
				if (this->bitIndex < 40)
				{
					char bitValue = deltaTime > 49 ? 1 : 0; // Low: 26-28us, High: 70us. (28+70)/2=49
					char dataIndex = this->bitIndex >> 3;
					volatile char* dataByte = this->data + dataIndex;
					*dataByte = ((*dataByte) << 1) | bitValue;
					this->bitIndex++;
					if (this->bitIndex == 40)
					{
						if (((this->data[0] + this->data[1] + this->data[2] + this->data[3]) & 0xFF) == this->data[4])
						{
							this->humidity = ((DHTSensorGetDataValueHandler*)this->getHumidityHandler)(this->data);
							this->temperature = ((DHTSensorGetDataValueHandler*)this->getTemperatureHandler)(this->data);
							this->state = DHTSensor::State_ReadSuccess;
							result = true;
						}
						else
							this->state = DHTSensor::State_ReadError;
					}
				}
			}

			break;
		}
	}

	this->changing = false;

	return result;
}

float DHTSensor::GetIdleTime()
{
	return (float)(Board::TimeMicros() - this->time) * 0.000001f;
}


float DHTSensor::GetTemperatureDHT11(const char* data)
{
	return (float)data[2] + ((float)data[3]) * 0.01f;
}

float DHTSensor::GetHumidityDHT11(const char* data)
{
	return (float)data[0] + ((float)data[1]) * 0.01f;
}


float DHTSensor::GetTemperatureDHT22(const char* data)
{
	return (float)((short)((data[2] << 8) | (data[3]))) * 0.1f;
}


float DHTSensor::GetHumidityDHT22(const char* data)
{
	return (float)((short)(((data[0] & 0x7f) << 8) | (data[1]))) * 0.1f;
}

