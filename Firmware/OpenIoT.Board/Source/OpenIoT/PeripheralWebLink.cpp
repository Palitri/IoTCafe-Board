//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralWebLink.h"

#include "HttpUtils.h"
#include "IWebClient.h"
#include "Math.h"
#include "Strings.h"
#include "Uid128.h"
#include "PropertyTransmissionProtocol.h"
#include "IPersistence.h"
#include "OpenIoTTransmissionProtocol.h"

#include "Log.h"

const unsigned int PeripheralWebLink::weblinkRequestIntervalMillis = 5000;
const unsigned int PeripheralWebLink::weblinkMaxResponseTimeMillis = 10000;

const char* PeripheralWebLink::webLinkBaseUrl = "/weblink/board/";
const char* PeripheralWebLink::webLinkRelaySignature = "\"relay\":\"";

PeripheralWebLink::PeripheralWebLink(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(3);

	this->responseRelayParser = new StreamingInputDetector();
	this->responseRelayParser->SetMatch(PeripheralWebLink::webLinkRelaySignature);

	this->isAwaitingResponse = false;
	this->hexIndex = -1;
}

PeripheralWebLink::~PeripheralWebLink()
{
	delete this->responseRelayParser;
}


int PeripheralWebLink::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->webFeed = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->webRelay = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->feedInterval = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));

	this->lastRequestTime = 0;
	this->lastFeedTime = 0;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralWebLink::Update()
{
	if (this->webRelay->bValue || this->webFeed->bValue)
	{
		unsigned long time = Board::TimeMillis();
		long deltaTime = time - this->lastRequestTime;

		if (deltaTime >= PeripheralWebLink::weblinkRequestIntervalMillis)
		{
			if (!this->isAwaitingResponse || (deltaTime >= PeripheralWebLink::weblinkMaxResponseTimeMillis))
			{
				bool feed = false;
				if (this->webFeed->bValue)
				{
					if (time - this->lastFeedTime >= (unsigned long)(this->feedInterval->fValue * 1000.0f))
					{
						feed = true;
						this->lastFeedTime = time;
					}
				}

				this->SendWeblinkRequest(this->webRelay->bValue, feed);
				this->lastRequestTime = time;
			}
		}

	}

	if (this->isAwaitingResponse)
	{
		this->ProcessRelayResponseInput();
	}
}

void PeripheralWebLink::SendWeblinkRequest(bool relay, bool feed)
{
	IWebClient* webClient = this->device->GetWebClient();
	if (webClient != null)
	{
		char* endpointUrl = this->GetRequestEndpoint();
		char* payload = this->GetRequestPayload(true, relay, feed);

		//Log::println(endpointUrl);
		//Log::println(payload);

		this->SetAwaitingResponseState(true);

		webClient->SendRequest(HttpUtils::httpServerAddress, endpointUrl, HttpUtils::httpPOST, payload, HttpUtils::httpServerPort);
	}
}

char* PeripheralWebLink::GetRequestEndpoint()
{
	IPersistence* persistence = this->device->GetPersistence();

	void* deviceUid;
	unsigned char deviceUidSize;
	persistence->GetValue(OpenIoTTransmissionProtocol::DevicePropertyId_DeviceUid, deviceUid, deviceUidSize);

	char* endpointUrl = Strings::GetBuffer(64);
	Strings::Copy(endpointUrl + Strings::Copy(endpointUrl, PeripheralWebLink::webLinkBaseUrl, false), Strings::ToHex(deviceUid, deviceUidSize));

	return endpointUrl;
}

char* PeripheralWebLink::GetRequestPayload(bool includeDeviceName, bool requestRelay, bool includeFeed)
{
	IPersistence* persistence = this->device->GetPersistence();

	char* payload = Strings::GetBuffer(512);
	int offset = 0;
	*(payload + offset++) = '{';

	void* uid;
	unsigned char uidSize;
	persistence->GetValue(OpenIoTTransmissionProtocol::DevicePropertyId_ProjectUid, uid, uidSize);
	offset += Strings::Copy(payload + offset, "\"pid\":\"", false);
	offset += Strings::Copy(payload + offset, Strings::ToHex(uid, uidSize), false);
	*(payload + offset++) = '"';

	persistence->GetValue(OpenIoTTransmissionProtocol::DevicePropertyId_UserUid, uid, uidSize);
	offset += Strings::Copy(payload + offset, ",\"uid\":\"", false);
	offset += Strings::Copy(payload + offset, Strings::ToHex(uid, uidSize), false);
	*(payload + offset++) = '"';

	if (includeDeviceName)
	{
		offset += Strings::Copy(payload + offset, ",\"dn\":\"", false);
		offset += Strings::Copy(payload + offset, (char*)persistence->GetValue(OpenIoTTransmissionProtocol::DevicePropertyId_DeviceName), false);
		*(payload + offset++) = '"';
	}
	
	if (requestRelay)
	{
		offset += Strings::Copy(payload + offset, ",\"relay\":{}", false);
	}

	if (includeFeed)
	{
		offset += Strings::Copy(payload + offset, ",\"feed\":{", false);
		offset += HttpUtils::GetPropertiesStatePayload(payload + offset, this->device->GetPropertiesObject(), false);
		offset += Strings::Copy(payload + offset, "}", false);
	}
	*(payload + offset++) = '}';
	*(payload + offset++) = 0;

	return payload;
}

void PeripheralWebLink::ProcessRelayResponseInput()
{
	IWebClient* webClient = this->device->GetWebClient();
	if (webClient != null)
	{
		PropertyTransmissionProtocol* p = this->device->GetTransmissionProtocol();

		char c;
		while (webClient->GetResponse(&c, 1) > 0)
		{
			if (this->isAwaitingResponse)
			{
				// TODO: Prone to errors - '{' and '}' chars may appear inside strings. Instead, check connection state
				if (c == '{')
					this->responseState++;
				else if ((c == '}'))
				{
					this->responseState--;

					if (this->responseState == 0)
						this->SetAwaitingResponseState(false);
				}
			}

			if (this->hexIndex == -1)
			{
				if (this->responseRelayParser->Feed(&c, 1) >= 0)
					this->hexIndex = 0;
			}
			else
			{
				if (c == '\"')
					this->hexIndex = -1;
				else
				{
					unsigned char hexValue;
					Strings::FromHex(&c, &hexValue, 1);
					if (this->hexIndex & 1)
					{
						this->hexByte |= hexValue;
						p->Feed(&this->hexByte, 1);
					}
					else
						this->hexByte = hexValue << 4;

					this->hexIndex++;
				}
			}
		}
	}
}

void PeripheralWebLink::SetAwaitingResponseState(bool isAwaiting)
{
	this->isAwaitingResponse = isAwaiting;
	this->responseState = 0;
	//Log::print("Awaiting Response: ");
	//Log::println((int)this->isAwaitingResponse);
}
