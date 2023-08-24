#ifndef PeripheralBrushlessMotor_h
#define PeripheralBrushlessMotor_h

#include "Peripheral.h"

#include "BrushlessMotor.h"

class PeripheralBrushlessMotor :
	public Peripheral
{
private:
	BrushlessMotor* motor;
	int pin;
	Property* pValue;

public:
	static const int Code_DeviceId = 15;

	PeripheralBrushlessMotor(IClusterDevice* device = null);
	virtual ~PeripheralBrushlessMotor();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralBrushlessMotor_h


