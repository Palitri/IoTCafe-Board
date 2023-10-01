//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "ATCommandUtils.h"

#include "Strings.h"

#include "SerialTransmissionChannel.h"

ATCommandUtils::ATCommandUtils(ITransmissionChannel* transmissionChannel)
{
	this->transmissionChannel = transmissionChannel;
}

ATCommandUtils::~ATCommandUtils()
{

}


ITransmissionChannel* ATCommandUtils::InitializeSerialWithUnknownBaud(int serialIndex, const int* baudCandidates, int numCandidates, int& foundBaud)
{
	SerialTransmissionChannel* serial = null;
	foundBaud = -1;

	for (int i = 0; i < numCandidates; i++)
	{
		if (serial != null)
			delete serial;

		foundBaud = baudCandidates[i];
		serial = new SerialTransmissionChannel(serialIndex, foundBaud);

		if (serial->WriteAwaitRetry("AT", "OK", 500, 2))
			break;
	}

	return serial;
}

//bool ATCommandUtils::SendHTTPRequest(const char* server, const char* path, const char* httpMethod, const char* payload, int port)
//{
//	// Enable HTTP / SSL mode (AT+HTTPINIT / AT+HTTPSSL=1)
//	this->transmissionChannel->WriteAwaitRetry("AT+HTTPINIT", "OK", 1000, 3);
//	
//	// Set HTTP bearer profile
//	this->transmissionChannel->WriteAwaitRetry("AT+HTTPPARA=\"CID\",1", "OK", 1000, 3);
//	
//	// Set URL to access
//	this->transmissionChannel->WriteAwaitRetry("AT+HTTPPARA=\"URL\",\"http://api.opencnc.info\"", "OK", 1000, 3);
//	
//	// Set content type
//	this->transmissionChannel->WriteAwaitRetry("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded", "OK", 1000, 3);
//	
//	// Set user data size
//	this->transmissionChannel->WriteAwaitRetry("AT+HTTPDATA=192,10000", "OK", 1000, 3);
//	
//	// Start HTTP GET Session
//	this->transmissionChannel->WriteAwaitRetry("AT+HTTPACTION=0", "OK", 1000, 3);
//	
//	// Read the HTTP response
//	this->transmissionChannel->WriteAwaitRetry("AT+HTTPREAD", "OK", 1000, 3);
//	
//	// Terminate HTTP
//	this->transmissionChannel->WriteAwaitRetry("AT+HTTPTERM", "OK", 1000, 3);
//}

bool ATCommandUtils::SendHTTPRequest(const char* server, const char* path, const char* httpMethod, const char* payload, int port)
{
	// Establish a TCP client connection. Examle: AT+CIPSTART="TCP","api.iot.cafe",80
	this->transmissionChannel->WriteText("AT+CIPSTART=\"TCP\",\"", false);
	this->transmissionChannel->WriteText(server, false);
	this->transmissionChannel->WriteText("\",", false);
	this->transmissionChannel->WriteText(Strings::ToDec(port), true);
	this->transmissionChannel->AwaitResponse("CONNECT", 3000);  // Use CONNECT instead of OK, because Sim800L produces OK first and CONNECT after, while Esp8266 produces CONNECT first and OK after. But CIPSEND fails if requested before CONNECT, even if after OK

	// Form HTTP headers to send. Example:"POST" +   " " + "/notifications" + " HTTP/1.1\r\nHost: " + "api.iot.cafe" + "\r\n\r\n"
	int requestLength = Strings::Length(httpMethod) + 1 + Strings::Length(path) + 17 + Strings::Length(server) + 4;

	int payloadLength = Strings::Length(payload);
	char* payloadLengthStr = 0;
	bool hasPayload = payloadLength > 0;
	if (hasPayload)
	{
		payloadLengthStr = Strings::ToDec(payloadLength);
		requestLength += 32;//Content-Type: application/json\r\n
		requestLength += 18;//Content-Length: \r\n
		requestLength += Strings::Length(payloadLengthStr);
		requestLength += payloadLength;
	}


	// Send data of a specified length
	this->transmissionChannel->WriteText("AT+CIPSEND=", false);
	this->transmissionChannel->WriteText(Strings::ToDec(requestLength), true);
	//this->transmissionChannel->Flush();
	Board::DelayMillis(200); // Found no instance of mentioning a delay here to be necessary, but doesn't work without it (gives out a "busy p..."). Using Flush() instead doesn't help

	this->transmissionChannel->WriteText(httpMethod, false);
	this->transmissionChannel->WriteText(" ", false);
	this->transmissionChannel->WriteText(path, false);
	this->transmissionChannel->WriteText(" HTTP/1.1\r\nHost: ", false);
	this->transmissionChannel->WriteText(server, false);
	this->transmissionChannel->WriteText("\r\n", false);
	if (hasPayload)
	{
		this->transmissionChannel->WriteText("Content-Type: application/json\r\n", false);
		this->transmissionChannel->WriteText("Content-Length: ", false);
		this->transmissionChannel->WriteText(payloadLengthStr, false);
		this->transmissionChannel->WriteText("\r\n", false);
	}
	this->transmissionChannel->WriteText("\r\n", false);
	if (hasPayload)
	{
		this->transmissionChannel->WriteText(payload, false);
	}

	this->transmissionChannel->AwaitResponse("OK", 5000);
	//this->transmissionChannel->AwaitResponse("SEND OK", 5000);

	// Close the TCP client connection
	//this->transmissionChannel->WriteText("AT+CIPCLOSE", true);
	//this->transmissionChannel->WriteAwaitRetry("AT+CIPCLOSE", "OK", 1000, 1);

	return true;
}
