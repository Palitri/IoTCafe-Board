//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PropertyTransmissionProtocol.h"

#include "Math.h"
#include "Mem.h"
#include "Strings.h"
#include "Board.h"

#include "Property.h"

#include "Log.h"

PropertyTransmissionProtocol::PropertyTransmissionProtocol(IPropertiesObject* propertiesObject) :
	CommandTransmissionProtocol()
{
	this->clientSubscribed = false;

	this->SetPropertiesObject(propertiesObject);
}

PropertyTransmissionProtocol::~PropertyTransmissionProtocol()
{
	this->SetPropertiesObject(null);
}

void PropertyTransmissionProtocol::ProcessInput()
{
	ChunkTransmissionProtocol::ProcessInput();
	
	this->SubscriptionRoutines();
}

void PropertyTransmissionProtocol::SetPropertiesObject(IPropertiesObject* propertiesObject)
{
	this->propertiesObject = propertiesObject;
}

void PropertyTransmissionProtocol::OnReceiveChunk(void* data, int dataSize)
{
	CommandTransmissionProtocol::OnReceiveChunk(data, dataSize);

	this->timeLastResponse = Board::TimeMillis();
}

bool PropertyTransmissionProtocol::OnReceiveCommand(unsigned char command, void* data, int dataSize)
{
	switch (command)
	{
		case ResponseCode_Ping:
		{
			break;
		}

		case ResponseCode_Info:
		{
			Log::println((char*)data, dataSize);

			break;
		}

		case CommandCode_Ping:
		{
			this->SendCommand(ResponseCode_Ping, data, dataSize);

			break;
		}

		case CommandCode_Info:
		{
			Log::println("Sending info");
			this->SendCommand(ResponseCode_Info, Board::name, Strings::Length(Board::name));

			break;
		}

		case CommandCode_QueryParamsInfo:
		{
			unsigned char propStart = *(unsigned char*)data;
			data = (unsigned char*)data + 1;
			unsigned char propCount = *(unsigned char*)data;
			data = (unsigned char*)data + 1;

			int propEnd = Math::Min(propStart + propCount, this->propertiesObject->GetPropertiesCount());

			int totalSize = 0;
			for (int propIndex = propStart; propIndex < propEnd; propIndex++)
			{
				Property* property = this->propertiesObject->GetProperty(propIndex);
				totalSize += 3 + this->GetPropertyFeedCommandSize(property);
			}

			this->BeginCommand(ResponseCode_QueryParamsInfo, totalSize);
			for (int propIndex = propStart; propIndex < propEnd; propIndex++)
			{
				Property* property = this->propertiesObject->GetProperty(propIndex);

				this->FeedCommand(&property->semantic, 1);
				this->FeedCommand(&property->type, 1);
				this->FeedCommand(&property->flags, 1);

				this->FeedCommandWithPropertyValue(property);
			}
			this->EndCommand();

			break;
		}

		case CommandCode_QueryParamsValues:
		{
			unsigned char propertyCount = *(unsigned char*)data;
			data = (unsigned char*)data + 1;

			int totalSize = 0;
			int returnedPropsCount = 0;
			for (int i = 0; i < propertyCount; i++)
			{
				unsigned char propertyIndex = *(unsigned char*)data;
				data = (unsigned char*)data + 1;

				Property* property = this->propertiesObject->GetProperty(propertyIndex);

				if (property->IsReadable())
				{
					totalSize += 1 + this->GetPropertyFeedCommandSize(property);
					returnedPropsCount++;
				}
			}

			data = (unsigned char*)data - propertyCount;
			this->BeginCommand(ResponseCode_QueryParamsValues, totalSize);
			for (int i = 0; i < propertyCount; i++)
			{
				unsigned char propertyIndex = *(unsigned char*)data;
				data = (unsigned char*)data + 1;

				Property* property = this->propertiesObject->GetProperty(propertyIndex);

				if (property->IsReadable())
				{
					this->FeedCommand(&propertyIndex, 1);

					this->FeedCommandWithPropertyValue(property);
				}
			}
			this->EndCommand();

			break;
		}

		case CommandCode_SetParamsValues:
		{
			//Log::println("Setting params values");

			unsigned char propertyCount = *(unsigned char*)data;
			data = (unsigned char*)data + 1;

			for (int i = 0; i < propertyCount; i++)
			{
				unsigned char propertyIndex = *(unsigned char*)data;
				data = (unsigned char*)data + 1;

				Property* property = this->propertiesObject->GetProperty(propertyIndex);

				int dataSize;
				if (property->type == PropertyType_Data)
				{
					dataSize = *(unsigned char*)data;
					data = (unsigned char*)data + 1;
				}
				else
					dataSize = property->Size();

				if (property->IsWritable())
					property->SetValue(data);

				data = (unsigned char*)data + dataSize;
			}

			this->SendCommand(ResponseCode_SetParamsValues);

			break;
		}

		case CommandCode_SetParamsSubscription:
		{
			bool subscribed = *(unsigned char*)data != 0;
			data = (unsigned char*)data + 1;
			unsigned char subscribedBit = subscribed ? PropertyFlag_Subscribed : 0;

			unsigned char propertyCount = *(unsigned char*)data;
			data = (unsigned char*)data + 1;

			for (int i = 0; i < propertyCount; i++)
			{
				unsigned char propertyIndex = *(unsigned char*)data;
				data = (unsigned char*)data + 1;

				Property* property = this->propertiesObject->GetProperty(propertyIndex);

				property->flags = (property->flags & ~PropertyFlag_Subscribed) | subscribedBit;
			}

			this->SendCommand(ResponseCode_SetParamsSubscription);

			break;
		}

		case CommandCode_ResetParamsSubscription:
		{
			this->ResetPropertiesSubscription();

			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}

void PropertyTransmissionProtocol::SendChangedSubscribedProperties()
{
	int propertiesCount = this->propertiesObject->GetPropertiesCount();

	if (propertiesCount == 0)
		return;

	int totalSize = 0;
	int returnedPropsCount = 0;

	for (int i = 0; i < propertiesCount; i++)
	{
		Property* property = this->propertiesObject->GetProperty(i);

		if (property->IsChanged())
		{
			if (property->IsSubscribed() && property->IsReadable())
			{
				totalSize += 1 + this->GetPropertyFeedCommandSize(property);
				returnedPropsCount++;
			}
		}
	}

	if (totalSize > 0)
	{
		totalSize += 1;
		this->BeginCommand(ResponseCode_SubscribedParamsChanged, totalSize);
		this->FeedCommand(&returnedPropsCount, 1);
		for (int i = 0; i < propertiesCount; i++)
		{
			Property* property = this->propertiesObject->GetProperty(i);

			bool isChanged = property->IsChanged();
			bool isSubscribed = property->IsSubscribed();

			if (isChanged)
			{
				if (isSubscribed && property->IsReadable())
				{
					this->FeedCommand(&i, 1);
					this->FeedCommandWithPropertyValue(property);
				}

				property->ResetChanged();
			}

			this->clientSubscribed |= isSubscribed;
		}
		this->EndCommand();
	}

}

void PropertyTransmissionProtocol::ResetPropertiesSubscription()
{
	unsigned int propertyCount = this->propertiesObject->GetPropertiesCount();
	for (int i = 0; i < propertyCount; i++)
	{
		Property* property = this->propertiesObject->GetProperty(i);
		property->flags = property->flags & ~PropertyFlag_Subscribed;
	}

	this->SendCommand(ResponseCode_ResetParamsSubscription);

	this->clientSubscribed = false;

	Log::println("ResetParamsSubscription");
}

void PropertyTransmissionProtocol::RequestPing()
{
	// TODO: Add time stamp on both sides
	this->SendCommand(CommandCode_Ping);
}

void PropertyTransmissionProtocol::SubscriptionRoutines()
{
	if (this->clientSubscribed)
	{
		unsigned long time = Board::TimeMillis();
		unsigned long timeIdle = time - this->timeLastResponse;

		if (timeIdle > PropertyTransmissionProtocol::Timeout_TransmissionIdle_Milliseconds)
		{
			this->ResetPropertiesSubscription();
		}

		else if (timeIdle > PropertyTransmissionProtocol::Timeout_StartPingAfterIdle_Milliseconds)
		{
			unsigned long timeAfterLastPing = time - this->timeLastPingRequest;
			if (timeAfterLastPing > PropertyTransmissionProtocol::Timeout_PingInterval_Milliseconds)
			{
				this->RequestPing();
				this->timeLastPingRequest = time;
			}
		}
	}
}

int PropertyTransmissionProtocol::GetPropertyFeedCommandSize(Property* property)
{
	return (property->type == PropertyType_Data ? 1 : 0) + property->Size();
}

void PropertyTransmissionProtocol::FeedCommandWithPropertyValue(Property* property)
{
	unsigned char propDataSize = property->Size();

	if (property->type == PropertyType_Data)
	{
		this->FeedCommand(&propDataSize, 1);
		this->FeedCommand(property->data, propDataSize);
	}
	else
		this->FeedCommand(&property->data, propDataSize);
}
