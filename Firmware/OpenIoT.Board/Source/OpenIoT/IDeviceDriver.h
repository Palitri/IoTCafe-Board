#ifndef IDeviceDriver_h
#define IDeviceDriver_h

/// <summary>
/// Represents a device, which can be driven iteratively by having each iteration define an action vector and then perform the action in multiple calls, each providing the andvancement of the action as a parameter
/// </summary>
class IDeviceDriver
{
public:
	/// <summary>
	/// Setups an iteration by providing an action vector
	/// </summary>
	/// <param name="origin">The start state of the action</param>
	/// <param name="vector">The change of the action state during the iteration</param>
	virtual void Begin(float origin, float vector) = 0;
	
	//  void End() = 0;
	
	/// <summary>
	/// Advances the action up to the provided phase
	/// </summary>
	/// <param name="phase">The phase (or progress) up to which to have the action performed, where 0 matches the origin state and 1 matches the end state</param>
	virtual void Drive(float phase) = 0;
};

#endif
// IDeviceDriver_h
