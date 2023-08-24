#ifndef OpenIoTCodeProcessor_h
#define OpenIoTCodeProcessor_h

#include "Property.h"
#include "CodeProcessor.h"
#include "IPropertiesObject.h"

#include "Math.h"

class OpenIoTCodeProcessor :
	public CodeProcessor
{
private:
	static const int CustomOperation_PropertyEvaluate = 1;
	static const int CustomOperation_PropertyAssign = 2;

	IPropertiesObject* propertiesObject;
public:
	OpenIoTCodeProcessor(IPropertiesObject* propertiesObject);

	void OnCallout(int calloutId);
};

#endif
// OpenIoTCodeProcessor_h