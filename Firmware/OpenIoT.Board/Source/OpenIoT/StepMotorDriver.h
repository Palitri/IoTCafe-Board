#ifndef StepMotorDriver_h
#define StepMotorDriver_h

class StepMotorDriver
{
private:
	int pinStep, pinDir;

	float stepsPerUnit;
	float distanceUnitsPerTurn;
	int fullStepsPerTurn;
	int stepMultiplier;

	bool stepState;
	bool forward;

	float vectorStepsTotal;
	int currentStep;

public:
	StepMotorDriver(int pinStep, int pinDir, float distanceUnitsPerTurn, int fullStepsPerTurn);
	~StepMotorDriver(void);

	float CalculateStepsPerUnit();
	float CalculateStepIntervalForSpeed(float speedUnitsPerSecond);

	void SetStepMultiplier(int stepMultiplier);
	void SetDirection(bool forward);
	void Pulse(bool forward, bool pulseState);
	void Pulse(bool forward);
	void Pulse();

	void Begin(float origin, float vector);
	void Drive(float phase);
};

#endif
// StepMotorDriver_h

