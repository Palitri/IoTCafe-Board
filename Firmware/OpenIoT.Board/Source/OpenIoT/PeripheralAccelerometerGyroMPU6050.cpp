//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralAccelerometerGyroMPU6050.h"

#include "Math.h"
#include "Timer.h"
PeripheralAccelerometerGyroMPU6050::PeripheralAccelerometerGyroMPU6050(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(10);
	this->mpuDevice = new AccelerometerGyroscopeMPU6050();
}

PeripheralAccelerometerGyroMPU6050::~PeripheralAccelerometerGyroMPU6050()
{
	delete this->mpuDevice;
}

int PeripheralAccelerometerGyroMPU6050::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->ax = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->ay = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->az = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->rx = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->ry = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->rz = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->rdx = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->rdy = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->rdz = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->temperature = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	unsigned char i2cIndex = *charCode++;
	this->mpuDevice->Init(i2cIndex);
	this->mpuDevice->Calibrate();

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralAccelerometerGyroMPU6050::Update()
{
	if (this->mpuDevice->UpdateReadings())
	{
		this->mpuDevice->ComputeRotation(this->device->GetTimer()->time);

		this->ax->SetFloat(this->mpuDevice->accelerometer.x);
		this->ay->SetFloat(this->mpuDevice->accelerometer.y);
		this->az->SetFloat(this->mpuDevice->accelerometer.z);
		this->rx->SetFloat(this->mpuDevice->rotation.x);
		this->ry->SetFloat(this->mpuDevice->rotation.y);
		this->rz->SetFloat(this->mpuDevice->rotation.z);
		this->rdx->SetFloat(this->mpuDevice->gyroscope.x);
		this->rdy->SetFloat(this->mpuDevice->gyroscope.y);
		this->rdz->SetFloat(this->mpuDevice->gyroscope.z);
		this->temperature->SetFloat(this->mpuDevice->temperature);
	}
}
