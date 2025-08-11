//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralAsynchronousDriver.h"

#include "Board.h"

#include "SineUnitMapper.h"
#include "Math.h"

PeripheralAsynchronousDriver::PeripheralAsynchronousDriver(IClusterDevice* device) :
	Peripheral(device)
{
}

PeripheralAsynchronousDriver::~PeripheralAsynchronousDriver()
{
}


int PeripheralAsynchronousDriver::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralAsynchronousDriver::Update()
{
}

void PeripheralAsynchronousDriver::ProcessCommand(char code, const char* data, int size)
{
	switch (code)
	{
		case PeripheralAsynchronousDriver::CommandCode_SetNumberOfChannels:
		{
			this->asyncEngine.SetNumberOfChannels(data[0]);
			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_SetChannelDevice:
		{
			char channel = data[0];
			char peripheralId = data[1];
			this->asyncEngine.SetChannelDevice(channel, this->device->GetPeripheral(peripheralId)->driver);
			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_SetChannelMapper:
		{
			Board::DelayMillis(1000);

			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_SetVector:
		{
			char channelId = *data;
			float vector = *(float*)(data + 1);

			this->asyncEngine.deviceChannels[channelId]->deviceDriver->Begin(0, vector);
			break;
		}

		case PeripheralAsynchronousDriver::CommandCode_Drive:
		{
			float time = *(float*)data;

			this->asyncEngine.Drive(time);
			break;
		}
	}
}

AsynchronousDeviceEngine* PeripheralAsynchronousDriver::GetAsyncEngine()
{
	return &this->asyncEngine;
}
