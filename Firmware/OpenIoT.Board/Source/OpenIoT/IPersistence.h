//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#ifndef IPersistence_h
#define IPersistence_h

class IPersistence
{
public:
	virtual void* GetValue(unsigned short key) = 0;
	virtual bool GetValue(unsigned short key, void*& data, unsigned char& size) = 0;
	virtual bool SetValue(unsigned short key, void* data, unsigned char size) = 0;
	virtual void Reset() = 0;
};

#endif
// IPersistence_h