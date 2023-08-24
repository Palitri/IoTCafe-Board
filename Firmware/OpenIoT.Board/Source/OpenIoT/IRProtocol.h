#ifndef IRProtocol_h
#define IRProtocol_h

class IRProtocol
{
protected:
	unsigned char protocolId;

public:
	IRProtocol();
	virtual ~IRProtocol();

	static IRProtocol* Dispatch(void* data);

	virtual int Configure(void* data) = 0;
	virtual void Encode(int pin, void* data, unsigned char dataLength) = 0;
	virtual void Repeat(int pin, void* data, unsigned char dataLength) = 0;
	virtual int GetRepeatTimeMicros() = 0;
};

#endif
// IRProtocol_h
