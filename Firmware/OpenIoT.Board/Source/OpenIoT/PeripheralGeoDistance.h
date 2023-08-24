#ifndef PeripheralGeoDistance_h
#define PeripheralGeoDistance_h

#include "Peripheral.h"

class PeripheralGeoDistance :
	public Peripheral
{
private:

public:
	static const int Code_DeviceId = 33;

	Property* latitude1, *longitude1, * latitude2, * longitude2, *distance;

	PeripheralGeoDistance(IClusterDevice* device = null);
	virtual ~PeripheralGeoDistance();

	static float CalculateGeoDistance(float lat1, float lon1, float lat2, float lon2, float altitude = 0.0f);
	static double CalculateGeoDistance2(double lat1, double lon1, double lat2, double lon2, double altitude = 0.0f);

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralGeoDistance_h