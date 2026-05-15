//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralAsynchronousDriver.h"

#include "Board.h"

#include "Math.h"

#include "UnitMapperDispatcher.h"

PeripheralAsynchronousDriver::PeripheralAsynchronousDriver(IClusterDevice* device) :
	Peripheral(device)
{
}

PeripheralAsynchronousDriver::~PeripheralAsynchronousDriver()
{
	for (int i = 0; i < this->asyncEngine.deviceChannels.count; i++)
		if (this->asyncEngine.deviceChannels[i]->unitMapper != null)
			delete this->asyncEngine.deviceChannels[i]->unitMapper;
}

int PeripheralAsynchronousDriver::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralAsynchronousDriver::Update()
{
}

bool PeripheralAsynchronousDriver::ProcessCommand(unsigned char command, void* data, int dataSize)
{
	switch (command)
	{
		case PeripheralAsynchronousDriver::CommandCode_SetNumberOfChannels:
		{
			this->asyncEngine.SetNumberOfChannels(*(unsigned char*)data);
			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_SetChannelDevice:
		{
			unsigned channel = *(unsigned char*)data;
			data += 1;
			unsigned char peripheralId = *(unsigned char*)data;
			
			if (channel >= this->asyncEngine.deviceChannels.count)
				break;

			this->asyncEngine.SetChannelDevice(channel, this->device->GetPeripheral(peripheralId)->driver);

			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_SetChannelMapper:
		{
			unsigned char channel = *(unsigned char*)data;
			data += 1;
			unsigned char mapperId = *(unsigned char*)data;

			if (channel >= this->asyncEngine.deviceChannels.count)
				break;

			if (this->asyncEngine.deviceChannels[channel]->unitMapper != null)
				delete this->asyncEngine.deviceChannels[channel]->unitMapper;

			this->asyncEngine.SetChannelMapper(channel, UnitMapperDispatcher::Dispatch(mapperId));

			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_SetChannelMapperParameters:
		{
			unsigned char channel = *(unsigned char*)data;
			if (channel >= this->asyncEngine.deviceChannels.count)
				break;

			this->asyncEngine.deviceChannels[channel]->unitMapper->Setup((void*)(data + 1));
		}

		case PeripheralAsynchronousDriver::CommandCode_SetVector:
		{
			unsigned char channelId = *(unsigned char*)data;
			float vector = *(float*)(data + 1);

			this->asyncEngine.deviceChannels[channelId]->deviceDriver->Begin(0, vector);
			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_ResetVectors:
		{
			for (int channelId = 0; channelId < this->asyncEngine.deviceChannels.count; channelId++)
				this->asyncEngine.deviceChannels[channelId]->deviceDriver->Begin(0, 0);

			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_Drive:
		{
			float time = *(float*)data;

			this->asyncEngine.Drive(time);
			break;
		}

		default:
		{
			return Peripheral::ProcessCommand(command, data, dataSize);
		}
	}

	return true;
}

AsynchronousDeviceEngine* PeripheralAsynchronousDriver::GetAsyncEngine()
{
	return &this->asyncEngine;
}
