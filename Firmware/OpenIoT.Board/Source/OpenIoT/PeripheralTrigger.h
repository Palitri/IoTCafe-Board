#ifndef PeripheralTrigger_h
#define PeripheralTrigger_h

#include "Peripheral.h"

class PeripheralTrigger :
	public Peripheral
{
private:
	unsigned long lastTriggeredTime;
	bool riseable, fallable, matchable;

public:
	static const int Code_DeviceId = 19;

	static const int Mode_OneTime = 0;
	static const int Mode_Continuous = 1;
	static const int Mode_Switch = 2;

	Property* input, * output;
	unsigned char mode;
	bool triggerOnRise, triggerOnFall, triggerOnMatch;
	float referenceValue, referenceThreshold;
	int delayTimeMillis;


	PeripheralTrigger(IClusterDevice* device = null);
	virtual ~PeripheralTrigger();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralTrigger_h
