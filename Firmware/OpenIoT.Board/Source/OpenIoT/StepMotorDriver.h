#ifndef StepMotorDriver_h
#define StepMotorDriver_h

class StepMotorDriver
{
private:
	int asyncPulsesRemaining;
	int asyncIntervalMicros;
	unsigned long asyncTime;
	bool asyncStepState;

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
	int GetRemainingAsyncSteps();
};

#endif
// StepMotorDriver_h

