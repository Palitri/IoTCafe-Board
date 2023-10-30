//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralDHT.h"

#include "DebugUtils.h"
#include "Math.h"

#include "Log.h"

PeripheralDHT* PeripheralDHT::interruptInstance;

PeripheralDHT::PeripheralDHT(IClusterDevice* device, int model) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);

	this->model = model;

	this->hasNewData = false;
	this->dhtSensor = null;
}

PeripheralDHT::~PeripheralDHT()
{
	if (this->dhtSensor != null)
		delete this->dhtSensor;
}

int PeripheralDHT::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->temperature = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->humidity = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	unsigned char pin = *charCode++;

	float updateInterval = *(float*)&charCode;
	charCode += 4;

	this->dhtSensor = new DHTSensor(pin, this->model == PeripheralDHT::Code_DeviceId_DHT22 ? DHTSensor::Model_DHT22 : DHTSensor::Model_DHT11);
	//Log::println("DHT Created");

	this->updateInterval = (long)(Math::Max(updateInterval, this->dhtSensor->minUpdateInterval) * 1000.0f);

	PeripheralDHT::interruptInstance = this;
	Board::AttachPinInterrupt(pin, BoardInterruptMode_Change, PeripheralDHT::InterruptHandler);

	this->lastUpdateTime = 0;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralDHT::Update()
{
	if (this->hasNewData)
	{
		//Log::println("New Data");

		this->temperature->SetFloat(this->dhtSensor->temperature);
		this->humidity->SetFloat(this->dhtSensor->humidity);

		this->hasNewData = false;
	}


	unsigned long time = Board::TimeMillis();
	long deltaTime = time - this->lastUpdateTime;

	if (deltaTime >= this->updateInterval)
	{
		//Log::print("State: ");
		//Log::println(this->dhtSensor->state);

		//Log::print("Bit: ");
		//Log::println(this->dhtSensor->bitIndex);

		//Log::println((int)this->dhtSensor->data[0]);
		//Log::println((int)this->dhtSensor->data[1]);
		//Log::println((int)this->dhtSensor->data[2]);
		//Log::println((int)this->dhtSensor->data[3]);
		//Log::println((int)this->dhtSensor->data[4]);

		//Log::print("T: ");
		//Log::println(this->dhtSensor->temperature);
		//Log::print("H: ");
		//Log::println(this->dhtSensor->humidity);

		//Log::println();

		this->dhtSensor->RequestUpdate();

		this->lastUpdateTime = time;
	}



	// TODO: Check why this->dhtSensor->GetIdleTime() breaks (produces 42
	//float idleTime = this->dhtSensor->GetIdleTime();
	//if (idleTime > this->updateInterval)
	//{
	//	Log::print("State: ");
	//	Log::println(this->dhtSensor->state);

	//	Log::print("Bit: ");
	//	Log::println(this->dhtSensor->bitIndex);

	//	Log::println((int)this->dhtSensor->data[0]);
	//	Log::println((int)this->dhtSensor->data[1]);
	//	Log::println((int)this->dhtSensor->data[2]);
	//	Log::println((int)this->dhtSensor->data[3]);
	//	Log::println((int)this->dhtSensor->data[4]);

	//	Log::print("T: ");
	//	Log::println(this->dhtSensor->temperature);
	//	Log::print("H: ");
	//	Log::println(this->dhtSensor->humidity);

	//	Log::println();

	//	this->dhtSensor->RequestUpdate();
	//}
}

void PeripheralDHT::OnInterrupt()
{
	this->hasNewData |= this->dhtSensor->OnChange();
}


void PeripheralDHT::InterruptHandler()
{
	PeripheralDHT::interruptInstance->OnInterrupt();
}