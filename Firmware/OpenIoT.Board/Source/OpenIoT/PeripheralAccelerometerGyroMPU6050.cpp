#include "PeripheralAccelerometerGyroMPU6050.h"

#include "Math.h"

#include "Log.h"

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
	Log::println("MPU6050 Loading");

	unsigned char* charCode = (unsigned char*)code;

	this->ax = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	Log::println("ax");
	this->ay = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	Log::println("ay");
	this->az = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	Log::println("az");
	this->rx = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	Log::println("rx");
	this->ry = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	Log::println("ry");
	this->rz = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	Log::println("rz");
	this->temperature = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	Log::println("temperature");

	unsigned char i2cIndex = *charCode++;

	this->device->Init(i2cIndex);
	Log::println("MPU6050 Initialized");


	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralAccelerometerGyroMPU6050::Update()
{
	Log::println("MPU6050 Updating");

	this->device->ReadData();
	Log::println("Read data");
	this->device->ComputeFloatVectors();
	Log::println("Float vectors");

	this->ax->SetFloat(this->device->accelerometer.x);
	this->ay->SetFloat(this->device->accelerometer.y);
	this->az->SetFloat(this->device->accelerometer.z);
	this->rx->SetFloat(this->device->gyroscope.x);
	this->ry->SetFloat(this->device->gyroscope.y);
	this->rz->SetFloat(this->device->gyroscope.z);
	this->temperature->SetFloat(this->device->temperature);
	Log::println("MPU6050 Updated");
}
