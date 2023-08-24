#ifndef Timer_h
#define Timer_h

class Timer
{
private:

	unsigned long timeAnchor;
	double lastSecondTime;
	int lastSecondTicks;


public:
	Timer();

	double speed, time, totalTime, actualTime, totalActualTime;
	int ticksLastSecond;
	unsigned long totalTicks;

	void Init();
	void Tick();
};

#endif
// Timer_h

