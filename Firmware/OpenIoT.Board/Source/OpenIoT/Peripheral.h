#ifndef Peripheral_h
#define Peripheral_h

#include "IClusterDevice.h"
#include "BasePropertiesObject.h"
#include "Board.h"

class Peripheral :
	public BasePropertiesObject
{
public:
	IClusterDevice* device;

	Peripheral();
	Peripheral(IClusterDevice* device);
	virtual ~Peripheral();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// Peripheral_h
