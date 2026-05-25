#ifndef PinDriver_h
#define PinDriver_h

#include "IDeviceDriver.h"

#include "Board.h"

class PinDriver :
	public IDeviceDriver
{
private:
	float origin;
	float vector;

public:
	int pin;
	BoardPinMode pinMode;

	PinDriver(int pin, BoardPinMode mode);
	~PinDriver(void);

	void SetFloat(float value);
	float GetFloat();
	void SetBool(bool value);
	bool GetBool();

	// IDeviceDriver
	void Begin(float origin, float vector);
	void Drive(float phase);
};

#endif
// PinDriver_h

