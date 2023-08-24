#ifndef BasePropertiesObject_h
#define BasePropertiesObject_h

#include "Property.h"
#include "IPropertiesObject.h"

class BasePropertiesObject :
	public IPropertiesObject
{
protected:
	int propertiesCapacity, propertiesCount;
	Property** properties;

public:
	BasePropertiesObject();
	virtual ~BasePropertiesObject();

	// IPropertiesObject
	virtual int GetPropertiesCount();
	virtual Property* GetProperty(int propertyIndex);

	virtual void SetPropertiesCapacity(int propertiesCapacity);
	virtual Property* AddProperty(Property* property);

	virtual void UnsubscribeAllProperties();
	virtual void UpdateProperties();
	virtual void ResetChangedPropertiesState();
};

#endif
// BasePropertiesObject_h
