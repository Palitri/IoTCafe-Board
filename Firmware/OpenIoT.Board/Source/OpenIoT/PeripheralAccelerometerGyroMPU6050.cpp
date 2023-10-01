//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "PeripheralAccelerometerGyroMPU6050.h"

#include "Math.h"

PeripheralAccelerometerGyroMPU6050::PeripheralAccelerometerGyroMPU6050(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(7);
	this->device = new AccelerometerGyroscopeMPU6050();
}

PeripheralAccelerometerGyroMPU6050::~PeripheralAccelerometerGyroMPU6050()
{
	delete this->device;
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
	this->temperature = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	unsigned char i2cIndex = *charCode++;
	this->device->Init(i2cIndex);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralAccelerometerGyroMPU6050::Update()
{
	this->device->ReadData();
	this->device->ComputeFloatVectors();

	this->ax->SetFloat(this->device->accelerometer.x);
	this->ay->SetFloat(this->device->accelerometer.y);
	this->az->SetFloat(this->device->accelerometer.z);
	this->rx->SetFloat(this->device->gyroscope.x);
	this->ry->SetFloat(this->device->gyroscope.y);
	this->rz->SetFloat(this->device->gyroscope.z);
	this->temperature->SetFloat(this->device->temperature);
}
