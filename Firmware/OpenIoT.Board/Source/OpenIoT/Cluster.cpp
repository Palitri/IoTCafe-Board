#include "Cluster.h"

#include "PeripheralSoftware.h"
#include "PeripheralPin.h"
#include "PeripheralServo.h"
#include "PeripheralUltrasonicSensor.h"
#include "PeripheralBrushlessMotor.h"
#include "PeripheralStepMotor.h"
#include "PeripheralRFID.h"
#include "PeripheralTrigger.h"
#include "PeripheralParameter.h"
#include "PeripheralTransition.h"
#include "PeripheralHlsToRgb.h"
#include "PeripheralInterpolation.h"
#include "PeripheralIRInput.h"
#include "PeripheralAnimation.h"
#include "PeripheralSine.h"
#include "PeripheralExponent.h"
#include "PeripheralPID.h"
#include "PeripheralIROutput.h"
#include "PeripheralWifiESP8266.h"
#include "PeripheralAccelerometerGyroMPU6050.h"
#include "PeripheralGpsNmea.h"
#include "PeripheralGeoDistance.h"
#include "PeripheralWebAlert.h"
//#include "PeripheralEmailNotification.h"
//#include "PeripheralSMSNotification.h"
#include "PeripheralWebLink.h"
#include "PeripheralDeviceDetails.h"
#include "PeripheralGPRS.h"
#include "PeripheralSMSMessage.h"
#include "PeripheralUSB.h"
#include "PeripheralBluetooth.h"
#include "PeripheralGenericWiFi.h"

#include "Board.h"

Peripheral* Cluster::CreatePeripheral(int deviceId, IClusterDevice *device)
{
	switch (deviceId)
	{
		case PeripheralSoftware::Code_DeviceId: // 11
			return new PeripheralSoftware(device);

		case PeripheralPin::Code_DeviceId: // 13
			return new PeripheralPin(device);

		case PeripheralServo::Code_DeviceId: // 14
			return new PeripheralServo(device);

		case PeripheralBrushlessMotor::Code_DeviceId: // 15
			return new PeripheralBrushlessMotor(device);

		case PeripheralUltrasonicSensor::Code_DeviceId: // 16
			return new PeripheralUltrasonicSensor(device);

		case PeripheralStepMotor::Code_DeviceId: // 17
			return new PeripheralStepMotor(device);

		case PeripheralRFID::Code_DeviceId: // 18
			return new PeripheralRFID(device);

		case PeripheralTrigger::Code_DeviceId: // 19
			return new PeripheralTrigger(device);

		case PeripheralParameter::Code_DeviceId: // 20
			return new PeripheralParameter(device);

		case PeripheralTransition::Code_DeviceId: // 21
			return new PeripheralTransition(device);

		case PeripheralHlsToRgb::Code_DeviceId: // 22
			return new PeripheralHlsToRgb(device);

		case PeripheralInterpolation::Code_DeviceId: // 23
			return new PeripheralInterpolation(device);

		case PeripheralIRInput::Code_DeviceId: // 24
			return new PeripheralIRInput(device);

		case PeripheralAnimation::Code_DeviceId: // 25
			return new PeripheralAnimation(device);

		case PeripheralSine::Code_DeviceId: // 26
			return new PeripheralSine(device);

		case PeripheralExponent::Code_DeviceId: // 27
			return new PeripheralExponent(device);

		case PeripheralPID::Code_DeviceId: // 28
			return new PeripheralPID(device);

		case PeripheralIROutput::Code_DeviceId: // 29
			return new PeripheralIROutput(device);

		case PeripheralWifiESP8266::Code_DeviceId: // 30
			return new PeripheralWifiESP8266(device);

		case PeripheralAccelerometerGyroMPU6050::Code_DeviceId: // 31
			return new PeripheralAccelerometerGyroMPU6050(device);

#ifndef BOARD_STM32F103C
		case PeripheralGpsNmea::Code_DeviceId: // 32
			return new PeripheralGpsNmea(device);

		case PeripheralGeoDistance::Code_DeviceId: // 33
			return new PeripheralGeoDistance(device);
#endif

		case PeripheralWebLink::Code_DeviceId: // 34
			return new PeripheralWebLink(device);

		case PeripheralWebAlert::Code_DeviceId: // 35
			return new PeripheralWebAlert(device);

		//case PeripheralEmailNotification::Code_DeviceId: // 36
		//	return new PeripheralEmailNotification(device);

		//case PeripheralSMSNotification::Code_DeviceId: // 37
		//	return new PeripheralSMSNotification(device);

		case PeripheralDeviceDetails::Code_DeviceId: // 38
			return new PeripheralDeviceDetails(device);

		case PeripheralGPRS::Code_DeviceId: // 39
			return new PeripheralGPRS(device);

		case PeripheralSMSMessage::Code_DeviceId: // 40
			return new PeripheralSMSMessage(device);

		case PeripheralUSB::Code_DeviceId: // 41
			return new PeripheralUSB(device);

		case PeripheralBluetooth::Code_DeviceId: // 42
			return new PeripheralBluetooth(device);

		case PeripheralGenericWiFi::Code_DeviceId: // 43
			return new PeripheralGenericWiFi(device);
	}

	return null;
}

Cluster::Cluster()
	: BasePropertiesObject()
{
	// TODO: Non-fixed property and peripheral sizes
	this->SetPropertiesCapacity(50);
	this->SetPeripheralsCapacity(20);
}

Cluster::~Cluster()
{
	this->Clear();
}

void Cluster::LoadCode(void* code, int codeSize, IClusterDevice *device)
{
	const unsigned char* charCode = (unsigned char*)code;
	int index = 0;

	while (index < codeSize)
	{
		Peripheral* clusterObject = Cluster::CreatePeripheral(charCode[index++], device);
		if (clusterObject != null)
		{
			index += clusterObject->Load((void*)((unsigned int)code + index));
			this->AddPeripheral(clusterObject);
		}
	}
}


void Cluster::SetPeripheralsCapacity(int capacity)
{
	this->objectsCapacity = capacity;
	this->objects = new Peripheral * [this->objectsCapacity];

	this->objectsCount = 0;
}

void Cluster::AddPeripheral(Peripheral* clusterObject, bool propertiesOnly)
{
	if (!propertiesOnly)
	{
		if (this->objectsCount >= this->objectsCapacity)
			return;

		this->objects[this->objectsCount++] = clusterObject;
	}

	int propertiesCount = clusterObject->GetPropertiesCount();
	for (int i = 0; i < propertiesCount; i++)
		this->AddProperty(clusterObject->GetProperty(i));
}

void Cluster::Update()
{
	for (int i = 0; i < this->objectsCount; i++)
	{
		this->objects[i]->Update();
	}
}

void Cluster::Clear()
{
	for (int i = 0; i < this->objectsCount; i++)
		delete this->objects[i];

	this->objectsCount = 0;

	this->propertiesCount = 0;
}
