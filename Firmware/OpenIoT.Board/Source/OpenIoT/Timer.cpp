#include "Timer.h"

#include "Board.h"

Timer::Timer()
{
	this->Init();
}

void Timer::Init()
{
	this->speed = 1.0;
	this->time = 0.0;
	this->totalTime = 0.0;
	this->actualTime = 0.0;
	this->totalActualTime = 0.0;
	this->ticksLastSecond = 0;
	this->totalTicks = 0;

	this->lastSecondTicks = 0;
	this->lastSecondTime = 0.0;

	this->timeAnchor = Board::TimeMicros();
}

void Timer::Tick()
{
	unsigned long timeAnchor = Board::TimeMicros();
	unsigned long elapsed = timeAnchor - this->timeAnchor;

	this->actualTime = (double)elapsed / 1000000.0;
	this->time = this->actualTime * this->speed;
	this->totalTime += this->time;
	this->totalActualTime += this->actualTime;
	this->totalTicks++;

	this->lastSecondTicks++;
	this->lastSecondTime += this->actualTime;
	if (this->lastSecondTime >= 1.0f)
	{
		this->ticksLastSecond = (double)this->lastSecondTicks / this->lastSecondTime;
		this->lastSecondTicks = 0;
		this->lastSecondTime = 0.0f;
	}

	this->timeAnchor = timeAnchor;
}


