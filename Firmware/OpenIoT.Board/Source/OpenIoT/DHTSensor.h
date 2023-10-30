//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#ifndef DHTSensor_h
#define DHTSensor_h

class DHTSensor
{
private:
	static const int WakeupSignalInterval_DHT11;
	static const int WakeupSignalInterval_DHT22;

	static const float MinUpdateInterval_DHT11;
	static const float MinUpdateInterval_DHT22;

	static const int InitializationInterval;

	static float GetTemperatureDHT11(const char* data);
	static float GetHumidityDHT11(const char* data);
	static float GetTemperatureDHT22(const char* data);
	static float GetHumidityDHT22(const char* data);

	void* getTemperatureHandler;
	void* getHumidityHandler;

	int wakeupSignalInterval;

public:
	static const char State_None = 0;
	static const char State_Initializing = 1;
	static const char State_Default = 2;
	static const char State_WakingUp = 3;
	static const char State_Reading = 4;
	static const char State_ReadSuccess = 5;
	static const char State_ReadError = 6;

	static const char Model_DHT11 = 1;
	static const char Model_DHT22 = 2;

	float minUpdateInterval;

	char state;

	char data[5];
	char bitIndex;
	volatile unsigned long time;
	volatile bool changing;





	char model;
	int pin;

	float temperature, humidity;

	DHTSensor(int pin, char dhtModel);
	virtual ~DHTSensor();

	void RequestUpdate();
	bool OnChange();

	float GetIdleTime();
};

#endif
// DHTSensor_h
