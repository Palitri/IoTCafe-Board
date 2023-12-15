//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Board.h"

#ifdef BOARD_ESP32

// ESP32-WROOM-DA Module. Set Minimal SPIFFS (Bare sketch with Wifi and BT uses 1368989 bytes(69 %))
// Configuration: HW Ready to go
// Built in led: 2 (GPIO2)


#include "Math.h"

#include "IClusterDevice.h"

#include "BoardESP32Features.h"

//const int Board::pwmBitResolution = 14;
//const int Board::pwmMaxValue = (1 << Board::pwmBitResolution) - 1;

const int Board::pwmBitResolution = 16; // 1 - 20 bits for ESP32, 1 - 14 bits for other ESPs
const int Board::pwmMaxValue = (1 << Board::pwmBitResolution) - 1;

const int Board::analogReadBitResolution = 12;
const int Board::analogReadMaxValue = (1 << Board::analogReadBitResolution) - 1;
const int Board::analogReadHalfValue = analogReadMaxValue / 2;

const int Board::eepromSize = 1024; // On ESP32, EEPROM.commit() works for up to 4096
const char* Board::name = "ESP32";

#include <Arduino.h>

#include <EEPROM.h> // Consider using ESP32's Preferences.h library
#include <Wire.h>

bool eepromInitialized = false;
BoardFeatures* boardFeatures = null;
TwoWire* wire = null;

const int pwmChannelsCount = 13;
const int pinMin = 0;
const int pinMax = 42;

int pwmPinChannels[pinMax - pinMin + 1];
int pwmChannelsUsed = 0;

unsigned char Board::nativeTriggerModesMap[5] =
{
  (unsigned char)RISING,
  (unsigned char)FALLING,
  (unsigned char)CHANGE,
  (unsigned char)LOW,
  (unsigned char)HIGH
};

void Board::Initialize(IClusterDevice* clusterDevice)
{
	if (boardFeatures != null)
		delete boardFeatures;

	boardFeatures = new BoardESP32Features(clusterDevice);

	pwmChannelsUsed = 0;
}

void* Board::DispatchFeaturesObject()
{
	return null;
}


bool Board::DigitalRead(int pin)
{
	return digitalRead(pin) == HIGH;
}

void Board::DigitalWrite(int pin, bool on)
{
	digitalWrite(pin, on ? HIGH : LOW);
}

float Board::AnalogRead(int pin)
{
	return (float)analogRead(pin) / (float)Board::analogReadMaxValue;
}

void Board::SetPwm(int pin, float duty)
{
	int value = Math::Trim((int)(duty * Board::pwmMaxValue), 0, Board::pwmMaxValue);
	int pwmChannel = pwmPinChannels[pin];
	ledcWrite(pwmChannel, value);

	//analogWrite(pin, value);
}

void Board::SetPinMode(int pin, BoardPinMode mode)
{
	const int pwmFrequency = 1200;

	int pwmChannel;
	ledcDetachPin(pin);
	pwmPinChannels[pin] = -1;


	switch (mode)
	{
	case BoardPinMode_DigitalInput:
		pinMode(pin, INPUT);
		break;

	case BoardPinMode_DigitalOutput:
		pinMode(pin, OUTPUT);
		break;

	case BoardPinMode_AnalogInput:
		// no action needed
		break;

	case BoardPinMode_PWMOutput:
		//pinMode(pin, OUTPUT);
		
		pwmChannel = pwmChannelsUsed;
		pwmChannelsUsed = (pwmChannelsUsed + 1) % pwmChannelsCount;
		pwmPinChannels[pin] = pwmChannel;

		ledcAttachPin(pin, pwmChannel);
		ledcSetup(pwmChannel, pwmFrequency, Board::pwmBitResolution);
		break;

	case BoardPinMode_DigitalInput_Pullup:
		pinMode(pin, INPUT_PULLUP);
		break;
	}
}

unsigned long Board::TimeMicros()
{
	return micros();
}

unsigned long Board::TimeMillis()
{
	return millis();
}

void Board::DelayMicros(unsigned long micros)
{
	delayMicroseconds(micros);
}

void Board::DelayMillis(unsigned long millis)
{
	delay(millis);
}

int Board::EEPROMRead(int address, void* destination, int size)
{
	if (!eepromInitialized)
	{
		EEPROM.begin(Board::eepromSize);
		eepromInitialized = true;
	}


	size = Math::Min(size, Board::eepromSize - address);

	if (size <= 0)
		return 0;

	int endAddress = address + size;
	while (address < endAddress)
		*(char*)destination++ = EEPROM.read(address++);

	return size;
}

int Board::EEPROMWrite(int address, const void* source, int size, bool force)
{
	if (!eepromInitialized)
	{
		EEPROM.begin(Board::eepromSize);
		eepromInitialized = true;
	}

	size = Math::Min(size, Board::eepromSize - address);

	if (size <= 0)
		return 0;

	int endAddress = address + size;
	char* src = (char*)source;
	while (address < endAddress)
		EEPROM.write(address++, *src++);

	return size;
}

void Board::EEPROMCommit()
{
	EEPROM.commit();
}

void* Board::SerialBegin(int serialIndex, int baudRate)
{
	HardwareSerial* serial = null;

	switch (serialIndex)
	{
	case 0:
		serial = &Serial;
		break;
	case 1:
		serial = &Serial1;
		break;
	case 2:
		serial = &Serial2;
		break;
	}

	serial->begin(baudRate);

	return serial;
}

int Board::SerialBytesAvailable(void* serial)
{
	return ((HardwareSerial*)serial)->available();
}

int Board::SerialRead(void* serial, const void* destination, int count)
{
	if (((HardwareSerial*)serial)->available() > 0)
		return ((HardwareSerial*)serial)->readBytes((char*)destination, count);

	return 0;
}

int Board::SerialWrite(void* serial, const void* source, int count)
{
	return ((HardwareSerial*)serial)->write((uint8_t*)source, count);
}

void Board::SerialFlush(void* serial)
{
	((HardwareSerial*)serial)->flush();
}


void *Board::I2CBegin(int i2cIndex, unsigned char address)
{
	if (wire != null)
		return wire;

	switch (i2cIndex)
	{
		case 0:
			wire = &Wire;
			break;
	}

	if (address == 0xFF)
		wire->begin();
	else
		wire->begin(address);

	return wire;
}

void Board::I2CEnd(void* i2c)
{
//	Wire.end();
}

int Board::I2CBytesAvailable(void* i2c)
{
	TwoWire* wire = (TwoWire*)i2c;

	return wire->available();
}

int Board::I2CReadMsbFirst(void* i2c, const void* destination, int count)
{
	TwoWire* wire = (TwoWire*)i2c;

	int result = 0;
	destination = destination + count;
	while ((wire->available() > 0) && (count > result))
	{
		destination = destination - 1;
		*((unsigned char*)destination) = wire->read();
		result++;
	}

	return result;
}

int Board::I2CReadLsbFirst(void* i2c, const void* destination, int count)
{
	TwoWire* wire = (TwoWire*)i2c;

	int result = 0;
	while ((wire->available() > 0) && (count > result))
	{
		*((unsigned char*)destination) = wire->read();
		destination = destination + 1;
		result++;
	}

	return result;
}

int Board::I2CWrite(void* i2c, const void* source, int count)
{
	TwoWire* wire = (TwoWire*)i2c;

	return wire->write((unsigned char*)source, count);
}

int Board::I2CRequestData(void* i2c, unsigned char address, int count)
{
	TwoWire* wire = (TwoWire*)i2c;

	wire->requestFrom((uint8_t)address, (uint8_t)count);

	return 0;
}

void Board::I2CBeginWrite(void* i2c, unsigned char address)
{
	TwoWire* wire = (TwoWire*)i2c;

	wire->beginTransmission(address);
}

void Board::I2CEndWrite(void* i2c)
{
	TwoWire* wire = (TwoWire*)i2c;

	wire->endTransmission();
}

void* Board::ServoBegin(int pin)
{
	//Servo* servo = new Servo();
	//servo->attach(pin);
	//return servo;

	return null;
}

void Board::ServoWrite(void* servo, int microseconds)
{
	//((Servo*)servo)->writeMicroseconds(microseconds);
}

void Board::AttachPinInterrupt(int pin, BoardInterruptMode mode, BoadInterruptHandler handler)
{
	attachInterrupt(digitalPinToInterrupt(pin), handler, nativeTriggerModesMap[(int)mode - 1]);
}

void Board::DetachPinInterrupt(int pin)
{
	detachInterrupt(digitalPinToInterrupt(pin));
}

void Board::Pulse(int pin, int timeMicros, int carrierFrequencyHz)
{
	// On STM32 delayMicroseconds actually delays half the time passed !?
	//const int timeCorrection = 1;
	int interval = 1000000 / carrierFrequencyHz;
	//int mod = 1000000 % carrierFrequencyHz;
	//int error = mod;
	int delay1 = interval - 1;
	int delay2 = delay1 - 2;

	unsigned long time = micros();
	while (micros() - time < timeMicros)
	{
		digitalWrite(pin, HIGH);
		delayMicroseconds(delay1);

		digitalWrite(pin, LOW);
		delayMicroseconds(delay2);

		//    error += mod;
		//    if (error >= carrierFrequencyHz)
		//    {
		//      delayMicroseconds(delay2 + 1);
		//      timeMicros -= interval + 1;
		//      error -= carrierFrequencyHz;
		//    }
		//    else
		//    {
		//      delayMicroseconds(delay2);
		//      timeMicros -= interval;
		//    }
	}
}

int Board::AwaitInput(int pin, bool value, int timeMicros)
{
	return pulseIn(pin, value ? HIGH : LOW, timeMicros);
}

#endif
