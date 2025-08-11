#ifndef PeripheralAsynchronousDriver_h
#define PeripheralAsynchronousDriver_h

#include "Peripheral.h"

#include "StepMotorDriver.h"

#include "AsynchronousDeviceEngine.h"

class PeripheralAsynchronousDriver :
	public Peripheral
{
private:
	static const int CommandCode_SetNumberOfChannels = 1;
	static const int CommandCode_SetChannelDevice = 2;
	static const int CommandCode_SetChannelMapper = 3;
	static const int CommandCode_SetVector = 4;
	static const int CommandCode_Drive = 5;
	
	AsynchronousDeviceEngine asyncEngine;

public:
	static const int Code_DeviceId = 47;


	PeripheralAsynchronousDriver(IClusterDevice* device = null);
	virtual ~PeripheralAsynchronousDriver();

	virtual int Load(const void* code);
	virtual void Update();

	virtual void ProcessCommand(char code, const char* data, int size);

	virtual AsynchronousDeviceEngine* GetAsyncEngine();
};

#endif
// PeripheralAsynchronousDriver_h
