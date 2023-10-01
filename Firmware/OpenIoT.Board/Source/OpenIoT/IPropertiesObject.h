//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

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


