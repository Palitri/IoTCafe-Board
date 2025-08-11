#ifndef OpenIoTTransmissionProtocol_h
#define OpenIoTTransmissionProtocol_h

#include "PropertyTransmissionProtocol.h"

#include "Cluster.h"
#include "CodeProcessor.h"

class OpenIoTTransmissionProtocol :
	public PropertyTransmissionProtocol
{
private:
	static const int CommandCode_SetDeviceProperties		= 0x41;
	static const int CommandCode_GetDeviceProperties		= 0x42;
	static const int CommandCode_UploadSchemeLogic			= 0x43;
	static const int CommandCode_UploadProgramLogic			= 0x44;
	static const int CommandCode_ResetLogic					= 0x45;
	static const int CommandCode_Reset						= 0x46;
	static const int CommandCode_ListCommands				= 0x47;
	static const int CommandCode_ExecuteCommand				= 0x48;
	//Add: static const int CommandCode_WriteUserInfo			= 0x;
	//Add: static const int CommandCode_ReadUserInfo			= 0x;

	static const int ResponseCode_SetDeviceProperties		= Code_ResponseBit | CommandCode_SetDeviceProperties;
	static const int ResponseCode_GetDeviceProperties		= Code_ResponseBit | CommandCode_GetDeviceProperties;
	static const int ResponseCode_UploadSchemeLogic			= Code_ResponseBit | CommandCode_UploadSchemeLogic;
	static const int ResponseCode_UploadProgramLogic		= Code_ResponseBit | CommandCode_UploadProgramLogic;
	static const int ResponseCode_ResetLogic				= Code_ResponseBit | CommandCode_ResetLogic;
	static const int ResponseCode_Reset						= Code_ResponseBit | CommandCode_Reset;
	static const int ResponseCode_ListCommands				= Code_ResponseBit | CommandCode_ListCommands;
	static const int ResponseCode_ExecuteCommand			= Code_ResponseBit | CommandCode_ExecuteCommand;
	//Add: static const int ResponseCode_WriteUserInfo		= Code_ResponseBit | CommandCode_WriteUserInfo;
	//Add: static const int ResponseCode_ReadUserInfo		= Code_ResponseBit | CommandCode_ReadUserInfo;

	Cluster* parentClusterObject;
	CodeProcessor* codeProcessor;

	void* codeData;
	unsigned int codeDataSize;

	IClusterDevice* clusterDevice;

	ITransmissionChannel* transmissionChannel;

public:
	static const int DevicePropertyId_DeviceUid			= 0x11;
	static const int DevicePropertyId_DeviceName		= 0x12;
	static const int DevicePropertyId_Password			= 0x13;
	static const int DevicePropertyId_ProjectUid		= 0x14;
	static const int DevicePropertyId_ProjectName		= 0x15;
	static const int DevicePropertyId_ProjectHash		= 0x16;
	static const int DevicePropertyId_UserUid			= 0x17;
	static const int DevicePropertyId_FirmwareName		= 0x21;
	static const int DevicePropertyId_FirmwareVendor	= 0x22;
	static const int DevicePropertyId_FirmwareVersion	= 0x23;
	static const int DevicePropertyId_BoardName			= 0x31;

	OpenIoTTransmissionProtocol(ITransmissionChannel* transmissionChannel, IPropertiesObject* propertiesObject, Cluster* parentClusterObject, CodeProcessor* codeProcessor, IClusterDevice* clusterDevice);

	virtual int Read(void* destination, int count);
	virtual int Write(const void* source, int count);

	virtual bool OnReceiveCommand(unsigned char command, void* data, int dataSize);

	void LoadSchemeLogic(void* data, int dataSize);
	void LoadProgramLogic(void* data, int dataSize);
	void Reset();

	virtual ITransmissionChannel* GetTransmissionChannel();
	virtual void SetTransmissionChannel(ITransmissionChannel* transmissionChannel);
};
#endif
// OpenIoTTransmissionProtocol_h
