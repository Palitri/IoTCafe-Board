#ifndef Peripheral_h
#define Peripheral_h

#include "IClusterDevice.h"
#include "IDeviceDriver.h"
#include "BasePropertiesObject.h"
#include "Board.h"

class Peripheral :
	public BasePropertiesObject
{
public:
	IClusterDevice* device;
	IDeviceDriver* driver;

	Peripheral();
	Peripheral(IClusterDevice* device);
	virtual ~Peripheral();

	virtual int Load(const void* code);
	virtual void Update();

	virtual void ProcessCommand(char code, const char* data, int size);
};

#endif
// Peripheral_h
