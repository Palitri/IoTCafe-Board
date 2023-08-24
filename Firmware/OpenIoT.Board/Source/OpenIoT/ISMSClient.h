#ifndef ISMSClient_h
#define ISMSClient_h

class ISMSClient
{
public:
	virtual void SendSMS(const char* message, const char* recepient) = 0;
};

#endif
// ISMSClient_h
