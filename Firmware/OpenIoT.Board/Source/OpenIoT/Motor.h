#ifndef Motor_h
#define Motor_h

class Motor
{
public:
	bool limitOutputValue;
	float threshold;
	float minRiseTime, minFallTime; // Limitations on how fast the motor can change speeds. maxSpeed = 1 / minTime. These limitations degrade the motor's responsiveness, but may protect from instant high load
	float traction;

	Motor();
	virtual ~Motor();

	virtual void SetMotorTraction(float motorTraction) = 0;

	virtual void SetTraction(float traction);
	virtual void SetTraction(float traction, float deltaTime);
};

#endif
// Motor_h

