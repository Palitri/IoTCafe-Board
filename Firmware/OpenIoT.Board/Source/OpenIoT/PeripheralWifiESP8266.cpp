#include "PeripheralWifiESP8266.h"

#include "Board.h"

#include "Strings.h"

#include "SerialTransmissionChannel.h"

#include "Log.h"

PeripheralWifiESP8266::PeripheralWifiESP8266(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(3);

	this->serial = null;
}

PeripheralWifiESP8266::~PeripheralWifiESP8266()
{
	if (this->device->GetWebClient() == this)
		this->device->SetWebClient(null);

	if (this->serial != null)
		delete this->serial;
}

ITransmissionChannel* PeripheralWifiESP8266::InitializeSerial(int serialIndex)
{
	// By default ESP8266 uses 115200 baud rate, but with 115200 errors occur when large data is to be passed (might be a buffer size issue - occurs when receiving bigger web responses with STM32).
	// That is why we try to set the baud rate to 9600 (if it's not set already)
	const int baudCandidates[2] = { 9600, 115200 };
	int baud;
	ITransmissionChannel* serial = ATCommandUtils::InitializeSerialWithUnknownBaud(serialIndex, baudCandidates, 2, baud);

	Log::println(baud);

	if (baud == 9600)
		return serial;

	Log::println("ESP8266: Setting baud rate via AT+UART_DEF");
	bool changed = serial->WriteAwaitRetry("AT+UART_DEF=9600,8,1,0,0", "OK", 500, 2);

	if (!changed)
	{
		Log::println("ESP8266: Setting baud rate via CIOBAUD");
		changed = serial->WriteAwaitRetry("AT+CIOBAUD=9600", "OK", 500, 2);
	}

	if (changed)
	{
		Log::println("ESP8266: Init with desired baud reate");
		delete serial;
		baud = baudCandidates[0];
		serial = new SerialTransmissionChannel(serialIndex, baud);
	}

	return serial;
}

int PeripheralWifiESP8266::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->on = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));
	this->networkName = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));
	this->networkPassword = this->AddProperty(new Property((void**)&charCode, PropertyType_Data));

	unsigned char serialIndex = *charCode++;

	this->old_on = this->on->bValue;

	this->device->SetWebClient((IWebClient*)this);

	this->serial = PeripheralWifiESP8266::InitializeSerial(serialIndex);
	//this->serial = new SerialTransmissionChannel(0, 9600);
	this->atCommandUtils = new ATCommandUtils(this->serial);
	//Board::DelayMillis(1000);

	//this->serial->println("AT+CWLAP=\"OpenIOT\"");
	// List all available wifi connections around
	//this->serial->println("AT+CWLAP");
	
	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralWifiESP8266::Update()
{
	//if (this->on->isChanged)
	if (this->old_on != this->on->bValue)
	{
		this->old_on = this->on->bValue;
		Log::println("Wifi power...");
		if (this->on->bValue)
		{
			this->serial->WriteText("AT+CWJAP=\"");
			this->serial->WriteText(this->networkName->GetStringData());
			this->serial->WriteText("\",\"");
			this->serial->WriteText(this->networkPassword->GetStringData());
			this->serial->WriteText("\"", true);
			this->serial->AwaitResponse("OK", 15000);
		}
		else
		{
			this->serial->WriteText("AT+CWQAP", true);
			this->serial->AwaitResponse("OK", 15000);
		}
		Log::println("...set");
	}

}


void PeripheralWifiESP8266::SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload, int port)
{
	this->atCommandUtils->SendHTTPRequest(server, path, httpMethod, payload, port);
}

int PeripheralWifiESP8266::GetResponse(char* dest, int readCount)
{
	return this->serial->Read(dest, readCount);
}

