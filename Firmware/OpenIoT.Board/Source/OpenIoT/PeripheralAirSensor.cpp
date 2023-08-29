#include "PeripheralAirSensor.h"

#include "Math.h"

PeripheralAirSensor::PeripheralAirSensor(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(3);
	this->device = new AirSensorBMX();
}

PeripheralAirSensor::~PeripheralAirSensor()
{
	delete this->device;
}

int PeripheralAirSensor::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->temperature = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->pressure = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->humidity = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	unsigned char i2cIndex = *charCode++;
	this->device->Init(i2cIndex);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralAirSensor::Update()
{
	this->device->ReadData();

	this->temperature->SetFloat(this->device->temperature);
	this->pressure->SetFloat(this->device->pressure / 100.0f);
	this->humidity->SetFloat(this->device->humidity);
}
