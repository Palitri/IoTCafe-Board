#ifndef ATCommandUtils_h
#define ATCommandUtils_h

#include "ITransmissionChannel.h"

class ATCommandUtils
{
public:
	ITransmissionChannel* transmissionChannel;

	ATCommandUtils(ITransmissionChannel* transmissionChannel);
	~ATCommandUtils();

	// Finds the first of the candidate bauds that work with the serial and returns an instance initialized with it
	// Returns a serial initialized with a working baud, if any. Otherwise returns a serial initialized with the last baud candidate
	static ITransmissionChannel* InitializeSerialWithUnknownBaud(int serialIndex, const int* baudCandidates, int numCandidates, int& foundBaud);
	
	bool SendHTTPRequest(const char* server, const char* path, const char* httpMethod, const char* payload, int port);
};

#endif
// ATCommandUtils_h
