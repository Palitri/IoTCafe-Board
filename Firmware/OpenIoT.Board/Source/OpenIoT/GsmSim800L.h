#ifndef GsmSim800L_h
#define GsmSim800L_h

#include "Board.h"

#include "ITransmissionChannel.h"


class GsmSim800L
{
private:
	static const char smsTerminationChar;

	ITransmissionChannel* serial;

public:
	GsmSim800L(ITransmissionChannel* serial = null);
	virtual ~GsmSim800L();

	void SetTransmissionChannel(ITransmissionChannel* serial);
	void Enable(const char* pin);
	void EnableGPRS(const char* apnName, const char* apnUser, const char* apnPassword);
	void Disable();

	void SendSMS(const char* recepient, const char* message);
};

#endif
// GsmSim800L_h
