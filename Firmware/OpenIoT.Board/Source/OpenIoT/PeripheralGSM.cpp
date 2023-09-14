#include "PeripheralGSM.h"

#include "Board.h"

#include "Strings.h"

#include "SerialTransmissionChannel.h"

#include "Log.h"

PeripheralGSM::PeripheralGSM(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(6);

	this->gsmDevice = new GsmSim800L();

	this->serial = null;
}

PeripheralGSM::~PeripheralGSM()
{
	if (this->device->GetWebClient() == this)
		this->device->SetWebClient(null);

	delete this->gsmDevice;

	if (this->serial != null)
	{
		delete this->serial;
	}
}


int PeripheralGSM::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->on = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->gprs = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->pin = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));
	Log::print("PIN: ");
	Log::println(this->pin->GetStringData());
	this->apn = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));
	Log::print("APN: ");
	Log::println(this->apn->GetStringData());
	this->apnUser = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));
	Log::print("SMS User: ");
	Log::println(this->apnUser->GetStringData());
	this->apnPassword = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));
	Log::print("SMS Password: ");
	Log::println(this->apnPassword->GetStringData());

	this->old_on = this->on->bValue;

	unsigned char serialIndex = *charCode++;

	//this->serial = new SerialTransmissionChannel(serialIndex, 115200);
	this->serial = new SerialTransmissionChannel(serialIndex, 9600);
	this->atCommandUtils = new ATCommandUtils(this->serial);
	this->gsmDevice->SetTransmissionChannel(this->serial);
	Board::DelayMillis(1000);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralGSM::Update()
{
	//if (this->on->isChanged)
	if (this->old_on != this->on->bValue)
	{
		this->old_on = this->on->bValue;
		Log::println("Init GSM...");
		if (this->on->bValue)
		{
			this->gsmDevice->Enable(this->pin->Size() > 0 ? this->pin->GetStringData() : null);

			this->device->SetSMSClient((ISMSClient*)this);

			if (this->gprs->bValue)
			{
				this->gsmDevice->EnableGPRS(
					this->pin->Size() > 0 ? this->apn->GetStringData() : null,
					this->apnUser->Size() > 0 ? this->apnUser->GetStringData() : null,
					this->apnPassword->Size() > 0 ? this->apnPassword->GetStringData() : null
				);

				this->device->SetWebClient((IWebClient*)this);
			}
		}
		else
		{
			this->gsmDevice->Disable();
		}
		Log::println("...set");
	}

	const int bufferSize = 4;
	char buffer[bufferSize];

	while (this->serial->AvailableInputBytes() > 0)
	{
		buffer[this->serial->Read(buffer, bufferSize - 1)] = 0;
		Log::print(buffer);
	}

}

void PeripheralGSM::SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload, int port)
{
	this->atCommandUtils->SendHTTPRequest(server, path, httpMethod, payload, port);
}

int PeripheralGSM::GetResponse(char* dest, int readCount)
{
	return 0;
}

void PeripheralGSM::SendSMS(const char* message, const char* recepient)
{
	this->gsmDevice->SendSMS(message, recepient);
}
