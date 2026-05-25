#ifndef Peripheral_h
#define Peripheral_h

#include "IClusterDevice.h"
#include "IDeviceDriver.h"
#include "BasePropertiesObject.h"
#include "ICommandProcessor.h"
#include "Board.h"

class Peripheral :
	public BasePropertiesObject,
	public ICommandProcessor
{
public:
	IClusterDevice* device;
	IDeviceDriver* driver;

	Peripheral();
	Peripheral(IClusterDevice* device);
	virtual ~Peripheral();

	virtual int Load(const void* code);
	virtual void Update();

	// ICommandProcessor
	virtual bool ProcessCommand(unsigned char command, void* data, int dataSize);
};

#endif
// Peripheral_h
