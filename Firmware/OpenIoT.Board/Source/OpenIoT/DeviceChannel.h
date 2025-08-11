#ifndef DeviceChannel_h
#define DeviceChannel_h

#include "IDeviceDriver.h"
#include "IUnitMapper.h"

class DeviceChannel
{
public:
	IDeviceDriver* deviceDriver;
	IUnitMapper* unitMapper;

	DeviceChannel(void);
	virtual ~DeviceChannel();

	void Run(float phase);
};

#endif
// DeviceChannel_h
