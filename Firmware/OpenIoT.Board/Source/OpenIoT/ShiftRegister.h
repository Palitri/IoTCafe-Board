#ifndef ShiftRegister_h
#define ShiftRegister_h

class ShiftRegister
{
public:
	int shiftRegSerialPin, shiftRegStoragePin, shiftRegClockPin;

	int bitCount;

	ShiftRegister(int shiftRegSerialPin, int shiftRegClockPin, int shiftRegStoragePin);
	~ShiftRegister();

	void SetBits(void* bits, int bitCount);
};

#endif
// ShiftRegister_h


