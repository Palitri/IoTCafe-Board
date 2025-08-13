#ifndef PeripheralStepMotor_h
#define PeripheralStepMotor_h

#include "Peripheral.h"

#include "StepMotorDriver.h"

#include "AsynchronousDeviceEngine.h"

class PeripheralStepMotor :
	public Peripheral
{
private:
	Property* pTurns, * pTurnsPerSec, *pTurn, *pAccelerationTime, *pDecelerationTime;
	StepMotorDriver* motor;
	
	float stepsPerTurn;

	bool oldTurn;
	float oldTurns;
	int stepsRemaining;

	AsynchronousDeviceEngine asyncDriver;

public:
	static const int Code_DeviceId = 17;

	PeripheralStepMotor(IClusterDevice* device = null);
	virtual ~PeripheralStepMotor();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralStepMotor_h


