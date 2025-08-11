#ifndef AsynchronousDeviceEngine_h
#define AsynchronousDeviceEngine_h

#include "DeviceChannel.h"
#include "List.h"

/// <summary>
/// Handles running asynchronously multiple device drivers
/// </summary>
class AsynchronousDeviceEngine
{
public:
	List<DeviceChannel*> deviceChannels;

	AsynchronousDeviceEngine();
	virtual ~AsynchronousDeviceEngine();

	void Drive(float time);

	// Hardcode a LinearMapper, SineMapper and a Bezier Mapper for use with Line, Arc and Bezier commands

	//noe void ResetTransformation();
	//no void SetTranformation(float matrix[12]);
	
	//void SetGeometryDimen

	void SetNumberOfChannels(int numChannels);
	void SetChannelMapper(int channel, int mapper);
	void SetChannelDevice(int channel, IDeviceDriver* driver);
};

#endif
// AsynchronousDeviceEngine_h
// todo: stepper, brushless, pwm, servo
// todo: rename to continuous/parallel - device?. Not "function" because it may have multiple values for the same input (circle or bezier)