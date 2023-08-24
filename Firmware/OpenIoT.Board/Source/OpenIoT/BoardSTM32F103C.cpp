#include "Board.h"

#ifdef BOARD_STM32F103C

// STM32F103C
// Configuration: Jumper Boot0 :1, Push button before flashing, Usb to TTL (RX/TX) to pins A9(TX)/A10(RX)

#include "Math.h"

#include "IClusterDevice.h"


// Note that if board eeprom is attempted to be used outside its available address space, consequent uses might fail, even for proper addresses
const int Board::eepromSize = 508;
const char* Board::name = "STM32F103C";

#include <Arduino.h>

#include <EEPROM.h>
#include <Wire.h>
#include <Servo.h>

// Does not seem to work with STM32
//#ifdef __arm__
//// should use uinstd.h to define sbrk but Due causes a conflict
//extern "C" char* sbrk(int incr);
//#else  // __ARM__
//extern char *__brkval;
//#endif  // __arm__
// 
//int freeMemory() {
//  char top;
//#ifdef __arm__
//  return &top - reinterpret_cast<char*>(sbrk(0));
//#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
//  return &top - __brkval;
//#else  // __arm__
//  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
//#endif  // __arm__
//}

// Implement WireBase::end(), because STM32 produces an error "libraries\Wire\utility\WireBase.cpp.o:(.rodata._ZTV8WireBase+0x14): warning: undefined reference to `WireBase::end()'"
void WireBase::end()
{
}

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
	pwmWrite(pin, (int)Math::Round(Math::Trim(duty, 0.0f, 1.0f) * (float)Board::pwmMaxValue));
	//analogWrite(pin, Math::Trim((int)(duty * Board::pwmMaxValue), 0, Board::pwmMaxValue));
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
		pinMode(pin, PWM);
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

// On STM32 EEPROM reads/writes 256 chunks of 16bit values (total of 512 bytes)
// Assuming Little-endian for the 16bit chunks, though endianness doesn't matter, as long it's read and writtin the same way
int Board::EEPROMRead(int address, void* destination, int size)
{
	size = Math::Min(size, Board::eepromSize - address);

	if (size <= 0)
		return 0;

	int index16 = address / 2;
	int bytesLeft = size;
	if (address & 1)
	{
		*(char*)destination = (char)(EEPROM.read(index16) >> 8);
		destination = (char*)destination + 1;
		index16++;
		bytesLeft--;
	}

	while (bytesLeft > 1)
	{
		*(short*)destination = (short)EEPROM.read(index16);
		destination = (char*)destination + 2;
		index16++;
		bytesLeft -= 2;
	}

	if (bytesLeft > 0)
		*(char*)destination = (char)EEPROM.read(index16);


	return size;
}

int Board::EEPROMWrite(int address, const void* source, int size, bool force)
{
	size = Math::Min(size, Board::eepromSize - address);

	if (size <= 0)
		return 0;

	int index16 = address / 2;
	int bytesLeft = size;
	short value;
	if (address & 1)
	{
		value = EEPROM.read(index16);
		short newValue = (value & 0x00FF) | ((*(char*)source) << 8);
		if (value != newValue)
			EEPROM.write(index16, newValue);
		source = (char*)source + 1;
		index16++;
		bytesLeft--;
	}

	while (bytesLeft > 1)
	{
		EEPROM.update(index16, *(short*)source);
		source = (char*)source + 2;
		index16++;
		bytesLeft -= 2;
	}

	if (bytesLeft > 0)
	{
		value = EEPROM.read(index16);
		short newValue = (value & 0xFF00) | (*(char*)source);
		if (value != newValue)
			EEPROM.write(index16, newValue);
	}

	return size;
}

void Board::EEPROMCommit()
{

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
		serial = &Serial2;
		break;
	case 2:
		serial = &Serial3;
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
	return ((HardwareSerial*)serial)->write(source, count);
}

void Board::SerialFlush(void* serial)
{
	((HardwareSerial*)serial)->flush();
}


void *Board::I2CBegin(int i2cIndex, unsigned char address)
{
	TwoWire* wire = null;
	
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
	Wire.end();
}

int Board::I2CBytesAvailable(void* i2c)
{
	return Wire.available();
}

int Board::I2CRead(void* i2c, const void* destination, int count)
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

int Board::I2CWrite(void* i2c, const void* source, int count)
{
	return Wire.write((unsigned char*)source, count);
}

int Board::I2CRequestData(void* i2c, unsigned char address, int count)
{
	Wire.requestFrom(address, count);
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
	attachInterrupt(digitalPinToInterrupt(pin), handler, (ExtIntTriggerMode)nativeTriggerModesMap[(int)mode - 1]);
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
