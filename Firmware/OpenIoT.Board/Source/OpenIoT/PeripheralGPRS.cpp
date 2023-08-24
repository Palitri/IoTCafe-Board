#include "PeripheralGPRS.h"

#include "Board.h"

#include "Strings.h"

#include "SerialTransmissionChannel.h"

#include "Log.h"

PeripheralGPRS::PeripheralGPRS(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(1);

	this->serial = null;
}

PeripheralGPRS::~PeripheralGPRS()
{
	if (this->device->GetWebClient() == this)
		this->device->SetWebClient(null);

	if (this->serial != null)
		delete this->serial;
}


int PeripheralGPRS::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->on = this->AddProperty(new Property((void**)&charCode, PropertyType_Bool));

	unsigned char serialIndex = *charCode++;

	this->old_on = this->on->bValue;

	this->device->SetWebClient((IWebClient*)this);
	this->device->SetSMSClient((ISMSClient*)this);

	//this->serial = new SerialTransmissionChannel(serialIndex, 115200);
	this->serial = new SerialTransmissionChannel(serialIndex, 9600);
	this->atCommandUtils = new ATCommandUtils(this->serial);
	Board::DelayMillis(1000);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralGPRS::Update()
{
	//if (this->on->isChanged)
	if (this->old_on != this->on->bValue)
	{
		this->old_on = this->on->bValue;
		Log::println("Init GPRS...");
		if (this->on->bValue)
		{
			// Check communication with module
			Log::println(this->serial->WriteAwaitRetry("AT", "OK", 1000, 3));

			// Set verbose mode (for debugging)
			Log::println(this->serial->WriteAwaitRetry("AT+CMEE=2", "OK", 1000, 3));

			// Display product identification information (something along the line of "SIM800 R14.18")
			//Log::println(this->serial->WriteAwaitRetry("ATI", "OK", 1000, 3));

			// Display signal quality
			//Log::println(this->serial->WriteAwaitRetry("AT+CSQ", "OK", 1000, 3));

			// Display firmware revision (something along the line of "Revision:1418B05SIM800L24")
			//Log::println(this->serial->WriteAwaitRetry("AT+CGMR", "OK", 1000, 3));

			// Set sleep mode. 0: Disabled, 1: Enabled, determined by DTR, 2: Auto, in which module goes to sleep after some idle time and to wake it, passing a (any) waking char, followed by a dealy of 100ms is required.
			//this->serial->WriteAwaitRetry("AT+CSCLK=0", "OK", 1000, 3);

			// Check if sim card is ready
			Log::println(this->serial->WriteAwaitRetry("AT+CPIN?", "OK", 1000, 3));

			// Set card pin
			//this->serial->WriteAwaitRetry("AT+CPIN=\"0000\"", "OK", 1000, 3);

			// Check if registered on cellural network (calls and sms)
			//this->serial->WriteAwaitRetry("AT+CREG?", "+CREG: 0,1", 3000, 3);

			// Check if registered on GPRS network (internet)
			this->serial->WriteAwaitRetry("AT+CGREG?", "+CGREG: 0,1", 3000, 3);

			//// Deactivate GPRS PDP context and shut down any previous connections. CSTT or SAPBR may not work without this here.. maybe?
			//Log::println(this->serial->WriteAwaitRetry("AT+CIPSHUT", "OK", 1000, 3));

			// Set Connection to GPRS by setting bearer Contype parameter. AT+SAPBR=<cmd_type>,<cid> where <cmd_type>: 0-Close bearer, 1-Open bearer, 2-Query bearer, 3-Set bearer parameters, 4-Get bearer parameters and <cid>:
			Log::println(this->serial->WriteAwaitRetry("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 1000, 3));

			// Set operator Access Point Name, username, password
			Log::println(this->serial->WriteAwaitRetry("AT+CSTT=\"internet.vivacom.bg\",\"VIVACOM\",\"VIVACOM\"", "OK", 1000, 3));
			// ...or alternatively (but it doesn't affect AT+CSTT?):
			//this->serial->WriteAwaitRetry("AT+SAPBR=3,1,\"APN\",\"internet.vivacom.bg\"", "OK", 1000, 3);
			//this->serial->WriteAwaitRetry("AT+SAPBR=3,1,\"USER\",\"VIVACOM\"", "OK", 1000, 3);
			//this->serial->WriteAwaitRetry("AT+SAPBR=3,1,\"PWD\",\"VIVACOM\"", "OK", 1000, 3);

			
			// Bring Up Wireless Connection with GPRS or CSD 
			//this->serial->WriteAwaitRetry("AT+CIICR", "OK", 1000, 3);

			// Enable GPRS by opening bearer
			Log::println(this->serial->WriteAwaitRetry("AT+SAPBR=1,1", "OK", 3000, 3));

			// Set multi-IP mode. 0 = Single IP connection, 1 = Multi IP conneciton
			//Log::println(this->serial->WriteAwaitRetry("AT+CIPMUX=0", "OK", 1000, 3));

			// Attach to GPRS. 0 = Detached, 1 = Attached
			Log::println(this->serial->WriteAwaitRetry("AT+CGATT=1", "OK", 1000, 3));

			// Set data transmission mode. 0 - Normal mode, returns SEND OK when data transmitted. 1 - Quick send mode, returns DATA ACCEPT:<n>,<length>, where <n> is the connection number (for multi IP-connection)
			Log::println(this->serial->WriteAwaitRetry("AT+CIPQSEND=0", "OK", 1000, 3));

			// Check IP Adress (Response: <cid>,<status>,<IP>)
			//this->serial->WriteAwaitRetry("AT+SAPBR=2,1", "OK", 1000, 3);
			// ... or alternatively (Response <IP>)
			//this->serial->WriteAwaitRetry("AT+CIFSR", "OK", 1000, 3);

			// Deactivate GPRS PDP context and shut down any previous connections. CIPSEND doesn't seem to work without this here.
			Log::println(this->serial->WriteAwaitRetry("AT+CIPSHUT", "OK", 1000, 3));
		}
		else
		{
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

void PeripheralGPRS::SendRequest(const char* server, const char* path, const char* httpMethod, const char* payload, int port)
{
	this->atCommandUtils->SendHTTPRequest(server, path, httpMethod, payload, port);
}

int PeripheralGPRS::GetResponse(char* dest, int readCount)
{
	return 0;
}

void PeripheralGPRS::SendSMS(const char* message, const char* recepient)
{
	// Not tested (due to lack of second SIM card to acknowledge that message was received)
	Log::println("Sending SMS...");

	// Set SMS message format. 0 - PDU, 1 - Text
	Log::println(this->serial->WriteAwaitRetry("AT+CMGF=1", "OK", 3000, 2));

	this->serial->WriteText("AT+CMGS=\"", false);
	this->serial->WriteText(recepient, false);
	this->serial->WriteText("\"\r\n", false);
	Board::DelayMillis(200); // Flush() doesn't seem to do the job. Same situation as with AT+CIPSEND for GPRS

	this->serial->WriteText(message, false);

	char terminationChar = 26;
	this->serial->WriteText(&terminationChar, false);

	Log::println((int)this->serial->AwaitResponse("OK", 10000));

	Log::println("SMS done");
}



//Get signal strength :
//AT + CSQ
//Response : +CSQ : <strength 0 - 31>, <BitErrorRate 0 - 7, 99> Where(1 shl value) / 10 < BER percentage < (1 shl(1 + value)) / 10 and [99] means unknown or undetectable
//	Bit error rate values :
//0->BER < 0.2 %
//	1 -> 0.2 % < BER < 0.4 %
//	2 -> 0.4 % < BER < 0.8 %
//	3 -> 0.8 % < BER < 1.6 %
//	4 -> 1.6 % < BER < 3.2 %
//	5 -> 3.2 % < BER < 6.4 %
//	6 -> 6.4 % < BER < 12.8 %
//	7 -> 12.8 % < BER
//	99 -> not known or not detectable
//
//
//	Check network registration state :
//AT + CREG ?
//Response : +CREG : <state>, <mode>
//Where the 1st number is :
//0 not registered, MT is not currently searching a new operator to register to
//1 registered, home network
//2 not registered, but MT is currently searching a new operator to register to
//3 registration denied
//4 unknown(e.g.out of GERAN / UTRAN / E - UTRAN coverage)
//5 registered, roaming
//6 registered for "SMS only", home network(applicable only when indicates E - UTRAN)
//7 registered for "SMS only", roaming(applicable only when indicates E - UTRAN)
//8 attached for emergency bearer services only(see NOTE 2) (not applicable)
//9 registered for "CSFB not preferred", home network(applicable only when indicates E - UTRAN)
//10 registered for "CSFB not preferred", roaming(applicable only when indicates E - UTRAN)
//The second number is :
//0 GSM
//1 GSM Compact
//2 UTRAN
//3 GSM w / EGPRS
//4 UTRAN w / HSDPA
//5 UTRAN w / HSUPA
//6 UTRAN w / HSDPA and HSUPA
//7 E - UTRAN
//
//
//List available mobile operators :
//AT + COPS = ?
//
//
//Sim card pin command :
//AT + CPIN ?
//AT + CPIN = <pin>
//
