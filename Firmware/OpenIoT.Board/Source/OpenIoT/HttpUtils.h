#ifndef HttpUtils_h
#define HttpUtils_h

class IPropertiesObject;

class HttpUtils
{
public:
	static const char* httpServerAddress;
	static const int httpServerPort;

	static const char* httpHeaderNewLine;
	
	static const char* httpPOST;
	static const char* httpGET;

	static int GetPropertiesStatePayload(char *dest, IPropertiesObject *properties, bool nullTerminate = true);
};

#endif
// HttpUtils_h
