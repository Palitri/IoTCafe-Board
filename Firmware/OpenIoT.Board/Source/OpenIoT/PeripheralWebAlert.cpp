//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralWebAlert.h"

#include "HttpUtils.h"
#include "Strings.h"
#include "Uid128.h"
#include "IWebClient.h"

//#include "Log.h"

//#include "StringStream.h"

const char* PeripheralWebAlert::notificationsEndpoint = "/webalert/";

PeripheralWebAlert::PeripheralWebAlert(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(4);

	this->endpoint = Strings::CreateMerge(PeripheralWebAlert::notificationsEndpoint, Uid128::ToString(this->device->GetUid()));
}

PeripheralWebAlert::~PeripheralWebAlert()
{
	Strings::Delete(this->endpoint);
}

// Alerts/messages might either have each separate peripheral (sms, email, notificaiton, etc) or be all inside a single peripheral as with the WebAlert
// Cons: If other types of notifications come around, they'd need to have either their separate peripherals or have this peripheral changed.
// Pros: It's good to be able to send multiple notifications at once (and in one web call)
int PeripheralWebAlert::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->send = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->pushNotification = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->email = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->sms = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));

	this->old_send = this->send->bValue;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralWebAlert::Update()
{
	if (this->old_send != this->send->bValue)
	{
		this->old_send = this->send->bValue;
		if (this->send->bValue)
			this->SendNotification();
	}
}

void PeripheralWebAlert::SendNotification()
{
	//IWebClient* webClient = this->device->GetWebClient();
	//if (webClient != null)
	//{
	//	int alertType = (this->pushNotification->bValue ? 1 << 0 : 0) | (this->email->bValue ? 1 << 1 : 0) | (this->sms->bValue ? 1 << 2 : 0);

	//	char* c = Strings::GetBuffer(256);
	//	HttpUtils::GetPropertiesStatePayload(c, this->device->GetPropertiesObject());

	//	StringStream* payload = new StringStream();
	//	payload->WriteString("{", false);
	//	payload->WriteString(c, false);
	//	payload->WriteString(",\"type\":", false);
	//	payload->WriteString(Strings::ToDec(alertType), false);
	//	payload->WriteString("}");

	//	Log::println(payload->ToString());

	//	webClient->SendRequest(HttpUtils::httpServerAddress, this->endpoint, HttpUtils::httpPOST, payload->ToString(), HttpUtils::httpServerPort);

	//	delete payload;
	//}


	IWebClient* webClient = this->device->GetWebClient();
	if (webClient != null)
	{
		int alertType = (this->pushNotification->bValue ? 1 << 0 : 0) | (this->email->bValue ? 1 << 1 : 0) | (this->sms->bValue ? 1 << 2 : 0);

		char* payload = Strings::GetBuffer(256);
		int payloadLength = 0;
		payload[payloadLength++] = '{';
		payloadLength += HttpUtils::GetPropertiesStatePayload(&payload[payloadLength], this->device->GetPropertiesObject(), false);
		payload[payloadLength++] = ',';
		payloadLength += Strings::Copy(&payload[payloadLength], "\"type\":");
		payloadLength += Strings::Copy(&payload[payloadLength], Strings::ToDec(alertType));
		payload[payloadLength++] = '}';
		payload[payloadLength] = 0;

		webClient->SendRequest(HttpUtils::httpServerAddress, this->endpoint, HttpUtils::httpPOST, payload, HttpUtils::httpServerPort);
	}
}
