#ifndef PeripheralGpsNmea_h
#define PeripheralGpsNmea_h

#include "Peripheral.h"

#include "GpsNmeaProtocol.h"
#include "ITransmissionChannel.h"


class PeripheralGpsNmea :
	public Peripheral
{
private:
	GpsNmeaProtocol* nmea;
	ITransmissionChannel* transmission;

public:
	static const int Code_DeviceId = 32;

	Property *latitude, *longitude, *altitude, *time;

	PeripheralGpsNmea(IClusterDevice* device = null);
	virtual ~PeripheralGpsNmea();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralGpsNmea_h
