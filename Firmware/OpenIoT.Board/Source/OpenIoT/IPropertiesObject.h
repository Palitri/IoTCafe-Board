#ifndef IPropertiesObject_h
#define IPropertiesObject_h

class Property;

class IPropertiesObject
{
public:
	virtual int GetPropertiesCount() = 0;
	virtual Property* GetProperty(int propertyIndex) = 0;
};

#endif
// IPropertiesObject_h


