#include "DeviceChannel.h"

#include "Board.h"

DeviceChannel::DeviceChannel(void)
{
	this->deviceDriver = null;
	this->unitMapper = null;
}

DeviceChannel::~DeviceChannel()
{
	//if (this->deviceDriver != null)
	//	delete this->deviceDriver;

	//if (this->unitMapper != null)
	//	delete this->unitMapper;
}

void DeviceChannel::Run(float phase)
{
	if (this->deviceDriver != null)
	{
		if (this->unitMapper != null)
			phase = this->unitMapper->Map(phase);

		this->deviceDriver->Drive(phase);
	}
}
