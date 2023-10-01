//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "OpenIoTPersistence.h"

#include "Math.h"
#include "Mem.h"
#include "Strings.h"

#include "OpenIoTTransmissionProtocol.h"

const char* OpenIoTPersistence::firmwareName = "OpenIoT";
const char* OpenIoTPersistence::firmwareVendor = "Palitri";
const char OpenIoTPersistence::firmwareVersion[4] = { 0, 9, 0, 0 };

OpenIoTPersistence::OpenIoTPersistence()
{
	this->stream = new EEPROMStream();

	if (!this->ReadBlock(0, &this->header, sizeof(EepromHeader)))
		this->Reset();
}

OpenIoTPersistence::~OpenIoTPersistence()
{
	delete this->stream;
}


bool OpenIoTPersistence::SetUid(void* dest, const void* src, int size)
{
	if (size != EepromHeader::UidLength)
		return false;

	Uid128::FromBytes(dest, src);
	
	return true;
}

bool OpenIoTPersistence::SetName(char* dest, const char* src, int size)
{
	size = Math::Min(size, EepromHeader::NameMaxLength);
	Mem::Copy(dest, src, size);
	dest[size] = 0;
	
	return true;
}

void* OpenIoTPersistence::GetValue(unsigned short key)
{
	void* data;
	unsigned char size;
	if (this->GetValue(key, data, size))
		return data;

	return null;
}

bool OpenIoTPersistence::GetValue(unsigned short key, void*& data, unsigned char& size)
{
	switch (key)
	{
		case OpenIoTTransmissionProtocol::DevicePropertyId_DeviceUid:
			//if ((this->header.flags & Flag_HasDeviceId) == 0)
			//	return false;
			data = &this->header.deviceId;
			size = EepromHeader::UidLength;
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_DeviceName:
			data = this->header.deviceName;
			size = Strings::Length((char*)data);
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_ProjectUid:
			//if ((this->header.flags & Flag_HasProjectId) == 0)
			//	return false;
			data = &this->header.projectId;
			size = EepromHeader::UidLength;
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_ProjectName:
			data = this->header.projectName;
			size = Strings::Length((char*)data);
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_ProjectHash:
			data = &this->header.projectHash;
			size = 4;
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_UserUid:
			//if ((this->header.flags & Flag_HasUserId) == 0)
			//	return false;
			data = &this->header.userId;
			size = EepromHeader::UidLength;
			break;


		case OpenIoTTransmissionProtocol::DevicePropertyId_FirmwareName:
			data = (char*)OpenIoTPersistence::firmwareName;
			size = Strings::Length((char*)data);
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_FirmwareVendor:
			data = (char*)OpenIoTPersistence::firmwareVendor;
			size = Strings::Length((char*)data);
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_FirmwareVersion:
			data = (char*)OpenIoTPersistence::firmwareVersion;
			size = 4;
			break;



		case OpenIoTTransmissionProtocol::DevicePropertyId_BoardName:
			data = (char*)Board::name;
			size = Strings::Length((char*)data);
			break;



		case OpenIoTPersistence::Key_ProjectScheme:
		{
			if (this->header.schemeSize <= 0)
				return false;

			size = this->header.schemeSize;
			data = Strings::GetBuffer(size);
			return this->ReadBlock(dataStartOffset, data, size);
			break;
		}

		case OpenIoTPersistence::Key_ProjectScript:
		{
			if (this->header.scriptSize <= 0)
				return false;

			size = this->header.scriptSize;
			data = Strings::GetBuffer(size);
			return this->ReadBlock(dataStartOffset + this->header.schemeSize + blockCRCSize, data, size);
			break;
		}

		default:
			return false;
	}

	return true;
}

bool OpenIoTPersistence::SetValue(unsigned short key, void* data, unsigned char size)
{
	bool result = false;
	switch (key)
	{
		case OpenIoTTransmissionProtocol::DevicePropertyId_DeviceUid:
			result = this->SetUid(&this->header.deviceId, data, size);
			if (result)
				this->header.flags |= Flag_HasDeviceId;
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_DeviceName:
			result = this->SetName(this->header.deviceName, (char*)data, size);
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_ProjectUid:
			result = this->SetUid(&this->header.projectId, data, size);
			if (result)
				this->header.flags |= Flag_HasProjectId;
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_ProjectName:
			result = this->SetName(this->header.projectName, (char*)data, size);
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_ProjectHash:
			result = size == 4;
			if (result)
				this->header.projectHash = *(unsigned int*)data;
			break;

		case OpenIoTTransmissionProtocol::DevicePropertyId_UserUid:
			result = this->SetUid(&this->header.userId, data, size);
			if (result)
				this->header.flags |= Flag_HasUserId;
			break;

		case OpenIoTPersistence::Key_ProjectScheme:
			this->header.schemeSize = size;
			this->header.scriptSize = 0;
			this->WriteBlock(dataStartOffset, data, size);
			result = true;
			break;

		case OpenIoTPersistence::Key_ProjectScript:
			this->header.scriptSize = size;
			this->WriteBlock(dataStartOffset + this->header.schemeSize + blockCRCSize, data, size);
			result = true;
			break;
	}

	if (result)
		this->UpdateHeader();

	return result;
}


void OpenIoTPersistence::UpdateHeader()
{
	this->WriteBlock(0, &this->header, sizeof(EepromHeader));
}

int OpenIoTPersistence::CalculateBlockCrc(const void* data, int size)
{
	return Math::CRC16(data, size);
}

void OpenIoTPersistence::WriteBlock(int offset, const void* data, int size)
{
	unsigned short dataCRC = this->CalculateBlockCrc(data, size);
	this->stream->Seek(offset, IStream::SeekMode_Absolute);
	this->stream->Write(data, size);
	this->stream->Write(&dataCRC, 2);

	this->stream->Flush();
}

bool OpenIoTPersistence::ReadBlock(int offset, void* data, int size)
{
	unsigned short eepromCRC;
	this->stream->Seek(offset, IStream::SeekMode_Absolute);
	this->stream->Read(data, size);
	this->stream->Read(&eepromCRC, 2);

	unsigned short dataCRC = this->CalculateBlockCrc(data, size);

	return eepromCRC == dataCRC;
}

void OpenIoTPersistence::Reset()
{
	Mem::Zero(&this->header, sizeof(EepromHeader));
		//Mem::Zero(&this->header.projectId, sizeof(EepromHeader) - (int)(&((EepromHeader*)0)->projectId));

	Strings::Copy(this->header.deviceName, "New");

	this->UpdateHeader();
}
