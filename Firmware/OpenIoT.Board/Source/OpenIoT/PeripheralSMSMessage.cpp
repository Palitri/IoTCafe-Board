//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralSMSMessage.h"

#include "Math.h"
#include "Log.h"

#include "ISMSClient.h"

PeripheralSMSMessage::PeripheralSMSMessage(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);
}

PeripheralSMSMessage::~PeripheralSMSMessage()
{

}


int PeripheralSMSMessage::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->send = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->recepient = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));

	Log::print("SMS Recepient: ");
	Log::println(this->recepient->GetStringData());

	this->old_send = this->send->bValue;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralSMSMessage::Update()
{
	if (this->send->bValue != this->old_send)
	{
		if (this->send->bValue)
		{
			ISMSClient *smsClient = this->device->GetSMSClient();
			if (smsClient != null)
			{
				smsClient->SendSMS(this->device->GetDeviceName(), this->recepient->GetStringData());
			}
		}

		this->old_send = this->send->bValue;
	}
}
