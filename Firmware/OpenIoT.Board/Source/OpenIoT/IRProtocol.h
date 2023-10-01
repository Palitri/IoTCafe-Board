//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

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
