#ifndef ITransmissionProtocol_h
#define ITransmissionProtocol_h

class ITransmissionProtocol
{
public:
	virtual int Read(const void* destination, int count) = 0;
	virtual int Write(const void* source, int count) = 0;
	virtual void Feed(const void* source, int count) = 0;
};

#endif
// ITransmissionProtocol_h
