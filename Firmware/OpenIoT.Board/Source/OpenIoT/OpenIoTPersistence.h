#ifndef OpenIoTPersistence_h
#define OpenIoTPersistence_h

#include "Uid128.h"
#include "EEPROMStream.h"
#include "IPersistence.h"

struct EepromHeader
{
	static const unsigned short NameMaxLength = 31;
	static const unsigned short UidLength = Uid128::size;

	Uid128 deviceId;
	char deviceName[NameMaxLength + 1];

	Uid128 projectId;
	char projectName[NameMaxLength + 1];
	unsigned int projectHash;
	Uid128 userId;
	unsigned short schemeSize;
	unsigned short scriptSize;

	unsigned short flags;
};

class OpenIoTPersistence :
	public IPersistence
{
private:
	static const char* firmwareName;
	static const char* firmwareVendor;
	static const char firmwareVersion[4];

	static const unsigned short Flag_HasDeviceId		= 1 << 0;
	static const unsigned short Flag_HasProjectId		= 1 << 1;
	static const unsigned short Flag_HasUserId			= 1 << 2;

	static const int blockCRCSize = 2;

	static const int dataStartOffset = sizeof(EepromHeader) + blockCRCSize;


	EEPROMStream* stream;

	bool SetUid(void* dest, const void* src, int size);
	bool SetName(char* dest, const char* src, int size);
	void UpdateHeader();
	int CalculateBlockCrc(const void* data, int size);
	void WriteBlock(int offset, const void* data, int size);
	bool ReadBlock(int offset, void* data, int size);

public:
	static const unsigned short Key_ProjectScheme	= 0x01;
	static const unsigned short Key_ProjectScript	= 0x02;

	EepromHeader header;

	OpenIoTPersistence();
	virtual ~OpenIoTPersistence();

	void* GetValue(unsigned short key);
	bool GetValue(unsigned short key, void*& data, unsigned char& size);
	bool SetValue(unsigned short key, void* data, unsigned char size);
	void Reset();
};

#endif
// OpenIoTPersistence_h