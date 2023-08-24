#ifndef PeripheralHlsToRgb_h
#define PeripheralHlsToRgb_h

#include "Peripheral.h"

class PeripheralHlsToRgb :
	public Peripheral
{
private:
	static float HueToRgb(float p, float q, float t);
	static void HlsToRgb(float h, float l, float s, float& r, float& g, float& b);

public:
	static const int Code_DeviceId = 22;

	Property* h, * l, * s, * r, * g, * b;

	PeripheralHlsToRgb(IClusterDevice* device = null);
	virtual ~PeripheralHlsToRgb();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralHlsToRgb_h


