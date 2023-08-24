#include "OpenIoT.h"

#include "SerialTransmissionChannel.h"
#include "Log.h"

OpenIoT* openIoT;
bool initialized = false;

void OpenIoTInitialize()
{
	if (initialized)
		return;

	Log::SetTransmissionChannel(new SerialTransmissionChannel(0, 9600));

	openIoT = new OpenIoT(new SerialTransmissionChannel(0, 9600));
	//openIoT = new OpenIoT(new SerialTransmissionChannel(1, 9600));

	initialized = true;
}

void OpenIoTRun()
{
	openIoT->Run();
}



OpenIoT::OpenIoT(ITransmissionChannel* transmissionChannel) :
	Cluster()
{
	//for (int i = 0; i < 5; i++)
	//{
	//	Board::DelayMillis(500);
	//	Log::println("Waking up..");
	//}

	this->wifiNetworkName = null;
	this->wifiNetworkPassword = null;

	this->persistence = new OpenIoTPersistence();

	this->timer = new Timer();
	this->lastUpdateTime = this->timer->totalTime;

	this->codeProcessor = new OpenIoTCodeProcessor((Cluster*)this);

	this->transmissionProtocol = new OpenIoTTransmissionProtocol(transmissionChannel, (Cluster*)this, (Cluster*)this, this->codeProcessor, (IClusterDevice*)this);

	this->transmissionProtocol->Reset();
}

void OpenIoT::Run()
{
	this->timer->Tick();

	this->transmissionProtocol->ProcessInput();

	this->Update();

	this->SendPropertyUpdates();

	this->codeProcessor->Process();
}

void OpenIoT::SendPropertyUpdates()
{
	if (this->timer->totalTime - this->lastUpdateTime >= this->updateInterval)
	{
		//Log::println("Updating properties");

		this->lastUpdateTime = this->timer->totalTime;

		this->transmissionProtocol->SendChangedSubscribedProperties();
	}
}

void OpenIoT::WriteEepRomProperties()
{
	return;

	bool writeToEepRom = false;

	for (int i = 0; i < this->propertiesCount; i++)
		if (this->properties[i]->IsChanged())
			if (this->properties[i]->IsEEPRom())
			{
				writeToEepRom = true;
			}

	if (writeToEepRom)
	{

	}
}

IPersistence* OpenIoT::GetPersistence()
{
	return this->persistence;
}

void* OpenIoT::GetUid()
{
	return &this->persistence->header.deviceId;
}

void OpenIoT::SetUpdateInterval(float seconds)
{
	this->updateInterval = seconds;
}


IWebClient* OpenIoT::GetWebClient()
{
	return  this->webClient;
}

void OpenIoT::SetWebClient(IWebClient* webClient)
{
	this->webClient = webClient;
}


ISMSClient* OpenIoT::GetSMSClient()
{
	return this->smsClient;
}

void OpenIoT::SetSMSClient(ISMSClient* smsClient)
{
	this->smsClient = smsClient;
}

ITransmissionChannel* OpenIoT::GetTransmissionChannel()
{
	return this->transmissionProtocol->GetTransmissionChannel();
}

void OpenIoT::SetTransmissionChannel(ITransmissionChannel* transmissionChannel)
{
	this->transmissionProtocol->SetTransmissionChannel(transmissionChannel);
}

PropertyTransmissionProtocol* OpenIoT::GetTransmissionProtocol()
{
	return this->transmissionProtocol;
}

Timer* OpenIoT::GetTimer()
{
	return this->timer;
}

IPropertiesObject* OpenIoT::GetPropertiesObject()
{
	return (Cluster*)this;
}

char* OpenIoT::GetDeviceName()
{
	return this->persistence->header.deviceName;
}


void OpenIoT::SetWifiCredentials(char* networkName, char* networkPassword)
{
	this->wifiNetworkName = networkName;
	this->wifiNetworkPassword = networkPassword;
}

void OpenIoT::GetWifiCredentials(char** networkName, char** networkPassword)
{
	*networkName = this->wifiNetworkName;
	*networkPassword = this->wifiNetworkPassword;
}

