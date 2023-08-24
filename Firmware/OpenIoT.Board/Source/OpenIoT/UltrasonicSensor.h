#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

class UltrasonicSensor
{
public:

	int pinTrigger, pinEcho;
	unsigned long echoDurationMicroseconds;
	unsigned long maxTimeoutMicroseconds;
	float soundSpeedMetersPerSecond;
	float distance;
	bool distanceAtLimit;

	static const int triggerPulseMicroseconds;
	static const float soundSpeedInDryAirMetersPerSecond;

	void Init(int pinTrigger, int pinEcho, float maxDistance);
	void SetMaxDistance(float maxDistance);
	void Sense();

	float GetDistanceTime(float travelDistanceMeters);
	float GetTimeDistance(float timeSeconds);
};

#endif
// UltrasonicSensor_h

