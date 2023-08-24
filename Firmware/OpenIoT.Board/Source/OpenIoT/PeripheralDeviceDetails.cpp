#include "PeripheralDeviceDetails.h"

#include "Timer.h"

PeripheralDeviceDetails::PeripheralDeviceDetails(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(3);
}

PeripheralDeviceDetails::~PeripheralDeviceDetails()
{
}

int PeripheralDeviceDetails::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->sleepInterval = this->AddProperty(new Property((void**)&charCode, PropertyType_Integer));
	this->updateInterval = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->fps = this->AddProperty(new Property((void**)&charCode, PropertyType_Integer, PropertyFlag_Read));

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralDeviceDetails::Update()
{
	// Consider moving sleep functionality to device, so that all peripheral processing happens uninterrupted
	if (this->sleepInterval->iValue > 0)
		Board::DelayMillis(this->sleepInterval->iValue);

	this->device->SetUpdateInterval(this->updateInterval->fValue);

	Timer* timer = this->device->GetTimer();
	if (timer != null)
		this->fps->SetInt(timer->ticksLastSecond);

}
