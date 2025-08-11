//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralCNC.h"

#include "Board.h"

#include "SineUnitMapper.h"

#include "PeripheralAsynchronousDriver.h"

PeripheralCNC::PeripheralCNC(IClusterDevice* device) :
	Peripheral(device)
{
}

PeripheralCNC::~PeripheralCNC()
{
}


int PeripheralCNC::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralCNC::Update()
{
}

void PeripheralCNC::ProcessCommand(char code, const char* data, int size)
{
	const float speed = 200.0f;

	switch (code)
	{
		case PeripheralCNC::CommandCode_SetAsyncDevice:
		{
			unsigned char peripheralId = *data;

			if (peripheralId > 3)
				break;

			PeripheralAsynchronousDriver* asyncDriverPeripheral = (PeripheralAsynchronousDriver*)this->device->GetPeripheral(peripheralId);
			if (asyncDriverPeripheral == null)
				break;
			
			this->cncDevice.SetAsyncEngine(asyncDriverPeripheral->GetAsyncEngine());

			break;
		}

		case PeripheralCNC::CommandCode_Polyline:
		{
			const int dimensionality = this->cncDevice.asyncEngine->deviceChannels.count;

			int offset = 0;

			float speed = *(float*)((unsigned int)data + offset);
			offset += 4;

			int numPoints = (size - 4) / (dimensionality * 4);

			this->cncDevice.PlotPolyline(speed, numPoints, (float*)((unsigned int)data + offset));

			break;
		}

		case PeripheralCNC::CommandCode_Bezier:
		{
			const int dimensionality = this->cncDevice.asyncEngine->deviceChannels.count;

			int offset = 0;

			float speed = *(float*)((unsigned int)data + offset);
			offset += 4;

			int numPoints = (size - 4) / (dimensionality * 4);

			this->cncDevice.PlotBezier(speed, numPoints, (float*)((unsigned int)data + offset));

			break;
		}

		case PeripheralCNC::CommandCode_Arc:
		{
			const int dimensionality = this->cncDevice.asyncEngine->deviceChannels.count;

			if (size < (2 + 2 * dimensionality) * 4)
				break;

			int offset = 0;

			float speed = *(float*)((unsigned int)data + offset);
			offset += 4;

			float startAngle = *(float*)((unsigned int)data + offset);
			offset += 4;
			float endAngle = *(float*)((unsigned int)data + offset);
			offset += 4;

			float* axisX = (float*)((unsigned int)data + offset);
			offset += dimensionality * 4;
			float* axisY = (float*)((unsigned int)data + offset);
			offset += dimensionality * 4;

			this->cncDevice.PlotArc(speed, startAngle, endAngle, axisX, axisY);

			break;
		}

		case PeripheralCNC::CommandCode_Wait:
		{
			float time = *(float*)data;
			Board::DelayMillis((int)(time * 1000.0f));
			break;
		}
	}
}
