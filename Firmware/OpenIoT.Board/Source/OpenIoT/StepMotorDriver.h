#ifndef StepMotorDriver_h
#define StepMotorDriver_h

#include "IDeviceDriver.h"

class StepMotorDriver :
	public IDeviceDriver
{
private:
	// Shared
	unsigned char rotatingDirection;

	// IDeviceDriver
	float drivePulsesTotal;
	int drivePulsesMade;
	bool driveStepPinValue;

	// Stepper common
	bool isRotating;
	float stepsPerSecond;
	unsigned long lastPulseTime;
	int pulseIntervalMicros;
	int rotationSign;

public:
	static const bool DirectionForward = false;
	static const bool DirectionBackward = true;

	int pinStep, pinDir;

	StepMotorDriver(int pinStep, int pinDir);
	~StepMotorDriver(void);

	void SetRotationSpeed(float stepsPerSecond);
	void StartRotation(float stepsPerSecond);
	void StopRotation();
	void SetupDirection();
	bool IsRotating();
	int RotationRoutines();

	// IDeviceDriver
	void Begin(float origin, float vector);
	void Drive(float phase);
};

#endif
// StepMotorDriver_h

