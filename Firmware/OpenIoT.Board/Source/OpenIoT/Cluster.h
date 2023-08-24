#ifndef Cluster_h
#define Cluster_h

#include "IClusterDevice.h"
#include "Peripheral.h"
#include "BasePropertiesObject.h"

#include "IWebClient.h"
#include "Timer.h"

#include "Board.h"

class Cluster :
	public BasePropertiesObject
{
private:
	static Peripheral* CreatePeripheral(int deviceId, IClusterDevice* device = null);

public:
	Peripheral** objects;
	int objectsCount, objectsCapacity;

	Cluster();
	virtual ~Cluster();

	void LoadCode(void* code, int codeSize, IClusterDevice* device);
	void AddPeripheral(Peripheral* propertiesObject, bool propertiesOnly = false);

	void SetPeripheralsCapacity(int capacity);
	void Update();
	void Clear();
};

#endif
// Cluster_h


