#include "Board.h"

#ifdef BOARD_ESP8266

// Generic ESP8266 Module. Also working (with less robust wifi connect): NodeMCU 1.0 (ESP-12E Module)
// Configuration (for module, not board): RST pin: 10kOhm Vcc, EN pin: 10kOhm Vcc, IO15 pin: 10kOhm Gnd. When flashing, power up with IO0 pin to Gnd

// Reference: WinDrive:\Users\[User]\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries


#include "Math.h"

#include "IClusterDevice.h"

#include "BoardESP8266Features.h"

const int Board::eepromSize = 4096;
const char* Board::name = "ESP8266";

#include <Arduino.h>

#include <EEPROM.h>
#include <Wire.h>
#include <Servo.h>

// Implement WireBase::end(), because STM32 produces an error "libraries\Wire\utility\WireBase.cpp.o:(.rodata._ZTV8WireBase+0x14): warning: undefined reference to `WireBase::end()'"
//void WireBase::end()
//{
//}

bool eepromInitialized = false;
BoardFeatures* boardFeatures = null;
TwoWire* wire = null;

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

	boardFeatures = new BoardESP8266Features(clusterDevice);
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
	//pwmWrite(pin, (int)Math::Round(Math::Trim(duty, 0.0f, 1.0f) * (float)Board::pwmMaxValue));
	analogWrite(pin, Math::Trim((int)(duty * Board::pwmMaxValue), 0, Board::pwmMaxValue));
}

void Board::SetPinMode(int pin, BoardPinMode mode)
{
	switch (mode)
	{
	case BoardPinMode_DigitalInput:
		pinMode(pin, INPUT);
		break;

	case BoardPinMode_DigitalOutput:
		pinMode(pin, OUTPUT);
		break;

	case BoardPinMode_AnalogInput:
		// No pinMode setting needed. Analog pins are only input
		break;

	case BoardPinMode_PWMOutput:
		pinMode(pin, OUTPUT);
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
	// On STM32 delayMicroseconds actually delays half the time passed !? (better resolution)
	delayMicroseconds(micros << 1);
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
	return Wire.available();
}

int Board::I2CReadMsbFirst(void* i2c, const void* destination, int count)
{
	int result = 0;
	destination = destination + count;
	while ((Wire.available() > 0) && (count > result))
	{
		destination = destination - 1;
		*((unsigned char*)destination) = Wire.read();
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
	return Wire.write((unsigned char*)source, count);
}

int Board::I2CRequestData(void* i2c, unsigned char address, int count)
{
	Wire.requestFrom(address, count);

	return 0;
}

void Board::I2CBeginWrite(void* i2c, unsigned char address)
{
	Wire.beginTransmission(address);
}

void Board::I2CEndWrite(void* i2c)
{
	Wire.endTransmission();
}

void* Board::ServoBegin(int pin)
{
	Servo* servo = new Servo();
	servo->attach(pin);
	return servo;
}

void Board::ServoWrite(void* servo, int microseconds)
{
	((Servo*)servo)->writeMicroseconds(microseconds);
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
