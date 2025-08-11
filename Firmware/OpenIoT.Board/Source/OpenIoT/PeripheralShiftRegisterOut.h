#ifndef PeripheralShiftRegisterOut_h
#define PeripheralShiftRegisterOut_h

#include "Peripheral.h"

#include "ShiftRegister.h"

class PeripheralShiftRegisterOut :
	public Peripheral
{
private:
	static const int CommandCode_SetBits = 1;

	ShiftRegister* shiftReg;

	Property* pBits;

	int bitCount;
	int bitsCRC;

public:
	static const int Code_DeviceId = 46;

	PeripheralShiftRegisterOut(IClusterDevice* device = null);
	virtual ~PeripheralShiftRegisterOut();

	virtual int Load(const void* code);
	virtual void Update();

	virtual void ProcessCommand(char code, const char* data, int size);
};

#endif
// PeripheralShiftRegisterOut_h
