#include "PeripheralSMSMessage.h"

#include "Math.h"

#include "ISMSClient.h"

PeripheralSMSMessage::PeripheralSMSMessage(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);
}

PeripheralSMSMessage::~PeripheralSMSMessage()
{

}


int PeripheralSMSMessage::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->send = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->code = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));

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
				smsClient->SendSMS("Test sms message", "+359886351254");
			}
		}

		this->old_send = this->send->bValue;
	}
}
