#ifndef PeripheralDHT_h
#define PeripheralDHT_h

#include "Peripheral.h"

#include "DHTSensor.h"

class PeripheralDHT :
	public Peripheral
{
private:
	static PeripheralDHT* interruptInstance;
	static void InterruptHandler();

	unsigned long lastUpdateTime;
	bool hasNewData;
	DHTSensor* dhtSensor;

	void OnInterrupt();

public:
	static const int Code_DeviceId_DHT11 = 36;
	static const int Code_DeviceId_DHT22 = 37;

	int model;

	long updateInterval;
	
	Property *temperature, *humidity;

	PeripheralDHT(IClusterDevice* device, int deviceId);
	virtual ~PeripheralDHT();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralDHT_h
