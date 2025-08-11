#ifndef PeripheralCNC_h
#define PeripheralCNC_h

#include "Peripheral.h"

#include "CNCDevice.h"

class PeripheralCNC :
	public Peripheral
{
private:
	static const int CommandCode_SetAsyncDevice = 1;
	static const int CommandCode_Polyline = 11;
	static const int CommandCode_Bezier = 12;
	static const int CommandCode_Arc = 13;
	static const int CommandCode_Wait = 14;
	
	CNCDevice cncDevice;

public:
	static const int Code_DeviceId = 48;

	PeripheralCNC(IClusterDevice* device = null);
	virtual ~PeripheralCNC();

	virtual int Load(const void* code);
	virtual void Update();

	virtual void ProcessCommand(char code, const char* data, int size);
};

#endif
// PeripheralCNC_h
