#ifndef StepMotorDriver_h
#define StepMotorDriver_h

#include "IDeviceDriver.h"

class StepMotorDriver :
	public IDeviceDriver
{
private:
	int asyncIntervalMicros;
	int asyncPulsesRemaining;
	unsigned long asyncTime;
	bool asyncStepState;

	float drivePulsesTotal;
	int drivePulsesMade;


	bool SetDirectionAndNormalizeParameters(int& steps, float& rate);

public:
	static const unsigned char DirectionForward = 0;
	static const unsigned char DirectionBackward = 1;

	int pinStep, pinDir;

	unsigned char direction;

	StepMotorDriver(int pinStep, int pinDir);
	~StepMotorDriver(void);

	void SetDirection(unsigned char direction);
	void Step();
	void Step(unsigned char direction);

	void RotateBySpeed(int steps, float stepsPerSecond);
	void RotateByTime(int steps, float seconds);

	void BeginAsyncRotateBySpeed(int steps, float stepsPerSecond, float minStepInterval = 0.0f);
	void RotateAsync();
	void SetRotateAsyncSpeed(float stepsPerSecnd);
	int GetRemainingAsyncSteps();

	// IDeviceDriver
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="origin">Disregarded</param>
	/// <param name="vector">Number of steps</param>
	void Begin(float origin, float vector);
	void Drive(float phase);
};

#endif
// StepMotorDriver_h

