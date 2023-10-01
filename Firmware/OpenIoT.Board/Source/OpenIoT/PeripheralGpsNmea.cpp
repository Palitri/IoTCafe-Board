//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralGpsNmea.h"

#include "SerialTransmissionChannel.h"

#include "Math.h"

#include "Log.h"

PeripheralGpsNmea::PeripheralGpsNmea(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(4);

	this->nmea = new GpsNmeaProtocol();
	this->transmission = null;
}

PeripheralGpsNmea::~PeripheralGpsNmea()
{
	delete this->nmea;

	if (this->transmission == null)
		delete this->transmission;
}


int PeripheralGpsNmea::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->latitude = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->longitude = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->altitude = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->time = this->AddProperty(new Property((void**)&charCode, PropertyType_Integer, PropertyFlag_Read));

	unsigned char serialIndex = *charCode++;

	this->transmission = new SerialTransmissionChannel(serialIndex, 9600);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralGpsNmea::Update()
{
	while (true)
	{
		int availableBytes = this->transmission->AvailableInputBytes();
		if (availableBytes > 0)
		{
			const unsigned char bufferSize = 4;
			char buffer[bufferSize];
			int bytesRead = this->transmission->Read(buffer, Math::Min(availableBytes, bufferSize));
			if (this->nmea->FeedData(buffer, 0, bytesRead))
			{
				this->latitude->SetFloat(this->nmea->latitude);
				this->longitude->SetFloat(this->nmea->longitude);
				this->altitude->SetFloat(this->nmea->altitude);
				this->time->SetInt(this->nmea->time);
			}
		}
		else
			break;
	}
}
