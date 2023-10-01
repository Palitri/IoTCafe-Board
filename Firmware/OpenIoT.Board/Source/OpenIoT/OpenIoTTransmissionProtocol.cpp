//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "OpenIoTTransmissionProtocol.h"

#include "Log.h"

#include "Math.h"

#include "Strings.h"
#include "Uid128.h"
#include "IPersistence.h"

#include "OpenIoTPersistence.h"

OpenIoTTransmissionProtocol::OpenIoTTransmissionProtocol(ITransmissionChannel* transmissionChannel, IPropertiesObject* propertiesObject, Cluster* parentClusterObject, CodeProcessor* codeProcessor, IClusterDevice* clusterDevice) :
	PropertyTransmissionProtocol(propertiesObject)
{
	this->parentClusterObject = parentClusterObject;
	this->codeProcessor = codeProcessor;

	this->codeData = null;
	this->codeDataSize = 0;

	this->clusterDevice = clusterDevice;

	this->transmissionChannel = transmissionChannel;
}


bool OpenIoTTransmissionProtocol::OnReceiveCommand(unsigned char command, void* data, int dataSize)
{
	switch (command)
	{
		//case CommandCode_Info:
		//{
		//	IPersistence *persistence = this->clusterDevice->GetPersistence();

		//	char* info = Strings::GetBuffer(256);
		//	int infoIndex = Strings::Copy(info, (char*)persistence->GetValue(DevicePropertyId_FirmwareName));
		//	infoIndex += Strings::Copy(info + infoIndex, "\r\nBoard: ");
		//	infoIndex += Strings::Copy(info + infoIndex, Board::name);
		//	infoIndex += Strings::Copy(info + infoIndex, "\r\nFirmware: v");
		//	infoIndex += Strings::Copy(info + infoIndex, Strings::ToHex(persistence->GetValue(DevicePropertyId_FirmwareVersion), 4));
		//	infoIndex += Strings::Copy(info + infoIndex, "\r\nVendor: ");
		//	infoIndex += Strings::Copy(info + infoIndex, (char*)persistence->GetValue(DevicePropertyId_FirmwareVendor));
		//	infoIndex += Strings::Copy(info + infoIndex, "\r\nDevice: ");
		//	infoIndex += Strings::Copy(info + infoIndex, (char*)persistence->GetValue(DevicePropertyId_DeviceName));
		//	infoIndex += Strings::Copy(info + infoIndex, "\r\Project: ");
		//	infoIndex += Strings::Copy(info + infoIndex, (char*)persistence->GetValue(DevicePropertyId_ProjectName));

		//	this->SendCommand(ResponseCode_Info, info, Strings::Length(info));

		//	Log::println("Sent info");

		//	break;
		//}

		case CommandCode_SetDeviceProperties:
		{
			Log::println("SetDeviceProperties");
			IPersistence* persistence = this->clusterDevice->GetPersistence();

			this->SendCommand(ResponseCode_SetDeviceProperties, data, dataSize);

			while (dataSize > 0)
			{
				unsigned char propertyId = *(unsigned char*)data;
				data = (char*) data + 1;
				unsigned char valueSize = *(unsigned char*)data;
				data = (char*)data + 1;

				persistence->SetValue(propertyId, data, valueSize);

				data = (char*)data + valueSize;
				dataSize -= 2 + valueSize;
			}

			break;
		}

		case CommandCode_GetDeviceProperties:
		{
			Log::println("GetDeviceProperties");

			IPersistence *persistence = this->clusterDevice->GetPersistence();

			int size = 0;
			int offset = 0;
			while (offset < dataSize)
			{
				unsigned char propertyId = ((unsigned char*)data)[offset++];

				void* propertyData;
				unsigned char propertySize;
				persistence->GetValue(propertyId, propertyData, propertySize);

				size += 2 + propertySize;
			}

			this->BeginCommand(ResponseCode_GetDeviceProperties, size);
			offset = 0;
			while (offset < dataSize)
			{
				unsigned char propertyId = ((unsigned char*)data)[offset++];

				void* propertyData;
				unsigned char propertySize;
				persistence->GetValue(propertyId, propertyData, propertySize);

				this->FeedCommand(&propertyId, 1);
				this->FeedCommand(&propertySize, 1);
				this->FeedCommand(propertyData, propertySize);
			}
			this->EndCommand();

			break;
		}

		case CommandCode_UploadSchemeLogic:
		{
			Log::print("Uploading Scheme Logic...");

			this->LoadSchemeLogic(data, dataSize);

			this->clusterDevice->GetPersistence()->SetValue(OpenIoTPersistence::Key_ProjectScheme, data, dataSize);

			this->SendCommand(ResponseCode_UploadSchemeLogic);

			Log::println("done");

			break;
		}

		case CommandCode_UploadProgramLogic:
		{
			Log::print("Uploading Program Logic...");

			this->LoadProgramLogic(data, dataSize);

			this->clusterDevice->GetPersistence()->SetValue(OpenIoTPersistence::Key_ProjectScript, data, dataSize);

			this->SendCommand(ResponseCode_UploadProgramLogic);

			Log::println("done");

			break;
		}

		case CommandCode_ResetLogic:
		{
			this->clusterDevice->GetPersistence()->Reset();
			this->LoadSchemeLogic(null, 0);

			this->SendCommand(ResponseCode_ResetLogic);

			break;
		}

		case CommandCode_Reset:
		{
			this->Reset();

			this->SendCommand(ResponseCode_Reset);

			break;
		}

		default:
			return PropertyTransmissionProtocol::OnReceiveCommand(command, data, dataSize);
	}

	return true;
}

void OpenIoTTransmissionProtocol::LoadSchemeLogic(void* data, int dataSize)
{
	Cluster* cluster = (Cluster*)this->propertiesObject;
	cluster->Clear();
	cluster->LoadCode(data, dataSize, this->clusterDevice);

	this->parentClusterObject->UnsubscribeAllProperties();

	this->SetPropertiesObject(this->propertiesObject);

	this->LoadProgramLogic(null, 0);

	Board::Initialize(this->clusterDevice); // TODO: Not good to have Initialize here. Either think of different approach for its utility or at least rename it
}

void OpenIoTTransmissionProtocol::LoadProgramLogic(void* data, int dataSize)
{
	if (dataSize != this->codeDataSize)
	{
		if (this->codeDataSize > 0)
			Mem::Free(this->codeData);

		this->codeData = dataSize > 0 ? Mem::Get(dataSize) : null;

		this->codeDataSize = dataSize;
	}

	Mem::Copy(this->codeData, data, dataSize);
	this->codeProcessor->SetMemory(this->codeData, this->codeDataSize);
}

void OpenIoTTransmissionProtocol::Reset()
{
	Log::println("LoadEEPROMConfiguration");

	IPersistence* persistence = this->clusterDevice->GetPersistence();

	void* data;
	unsigned char size;

	if (persistence->GetValue(OpenIoTPersistence::Key_ProjectScheme, data, size))
		this->LoadSchemeLogic(data, size);

	if (persistence->GetValue(OpenIoTPersistence::Key_ProjectScript, data, size))
		this->LoadProgramLogic(data, size);
}

int OpenIoTTransmissionProtocol::Read(void* destination, int count)
{
	int readBytes = Math::Min(this->transmissionChannel->AvailableInputBytes(), count);
	return this->transmissionChannel->Read(destination, readBytes);
}

int OpenIoTTransmissionProtocol::Write(const void* source, int count)
{
	return this->transmissionChannel->Write(source, count);
}

ITransmissionChannel* OpenIoTTransmissionProtocol::GetTransmissionChannel()
{
	return this->transmissionChannel;
}

void OpenIoTTransmissionProtocol::SetTransmissionChannel(ITransmissionChannel* transmissionChannel)
{
	this->transmissionChannel = transmissionChannel;
}

