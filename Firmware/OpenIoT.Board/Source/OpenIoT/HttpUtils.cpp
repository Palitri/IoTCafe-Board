//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "HttpUtils.h"

#include "IPropertiesObject.h"
#include "Property.h"

#include "Strings.h"

const char* HttpUtils::httpServerAddress = "api.iot.cafe";
const int HttpUtils::httpServerPort = 80;
//const char* HttpUtils::httpServerAddress = "192.168.1.100";
//const int HttpUtils::httpServerPort = 82;

const char* HttpUtils::httpHeaderNewLine = "\r\n";

const char* HttpUtils::httpPOST = "POST";
const char* HttpUtils::httpGET = "GET";


int HttpUtils::GetPropertiesStatePayload(char* dest, IPropertiesObject* properties, bool nullTerminate)
{
	char* origin = dest;

	int propertiesCount = properties->GetPropertiesCount();

	dest += Strings::Copy(dest, "\"s\":[", false);
	for (int i = 0; i < propertiesCount; i++)
	{
		Property* prop = properties->GetProperty(i);
		dest += Strings::ToDec(prop->semantic, dest, false);

		if (i < propertiesCount - 1)
			*(dest++) = ',';
	}
	dest += Strings::Copy(dest, "],\"v\":[", false);

	for (int i = 0; i < propertiesCount; i++)
	{
		Property* prop = properties->GetProperty(i);
		{
			if (prop->type == PropertyType_Data)
				*(dest++) = '\"';
			dest += prop->ToString(dest, false);
			if (prop->type == PropertyType_Data)
				*(dest++) = '\"';

			if (i < propertiesCount - 1)
				*(dest++) = ',';
		}
	}

	*(dest++) = ']';

	if (nullTerminate)
		*(dest) = 0;

	return (unsigned int)dest - (unsigned int)origin;
}

//int HttpUtils::GetPropertiesStatePayload(char* dest, IPropertiesObject* properties, bool nullTerminate)
//{
//	int index = 0;
//
//	dest[index++] = '{';
//
//	int propertiesCount = properties->GetPropertiesCount();
//	for (int i = 0; i < propertiesCount; i++)
//	{
//		Property* prop = properties->GetProperty(i);
//		//if (prop->IsSubscribed())
//		{
//			dest[index++] = '\"';
//			dest[index++] = 's';
//			index += Strings::Copy(dest + index, Strings::ToDec(prop->semantic), false);
//			dest[index++] = '\"';
//			dest[index++] = ':';
//			if (prop->type == PropertyType_Data)
//				dest[index++] = '\"';
//			index += prop->ToString(dest + index, false);
//			if (prop->type == PropertyType_Data)
//				dest[index++] = '\"';
//
//			if (i < propertiesCount - 1)
//				dest[index++] = ',';
//		}
//	}
//
//	dest[index++] = '}';
//
//	if (nullTerminate)
//		dest[index++] = 0;
//
//	return index;
//}

