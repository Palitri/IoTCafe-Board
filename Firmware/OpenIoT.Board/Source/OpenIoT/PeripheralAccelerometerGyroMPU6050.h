#ifndef PeripheralAccelerometerGyroMPU6050_h
#define PeripheralAccelerometerGyroMPU6050_h

#include "Peripheral.h"

#include "AccelerometerGyroscopeMPU6050.h"

class PeripheralAccelerometerGyroMPU6050 :
	public Peripheral
{
private:
	AccelerometerGyroscopeMPU6050* device;

public:
	static const int Code_DeviceId = 31;

	Property *ax, *ay, *az, *rx, *ry, *rz, *temperature;

	PeripheralAccelerometerGyroMPU6050(IClusterDevice* device = null);
	virtual ~PeripheralAccelerometerGyroMPU6050();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralAccelerometerGyroMPU6050_h
