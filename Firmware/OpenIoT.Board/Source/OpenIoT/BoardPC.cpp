#include "Board.h"

#ifdef BOARD_PC

#include "Math.h"
#include "IClusterDevice.h"

#include <iostream>

const int Board::eepromSize = 512;
const char* Board::name = "PC";

unsigned char Board::nativeTriggerModesMap[5] = { 0, 1, 2, 3, 4 };

short* eeprom = new short[Board::eepromSize / 2];

void Board::Initialize(IClusterDevice* clusterDevice)
{

}

bool Board::DigitalRead(int pin)
{
	return false;
}

void Board::DigitalWrite(int pin, bool on)
{
}

float Board::AnalogRead(int pin)
{
	return 0.0f;
}

void Board::SetPwm(int pin, float duty)
{
}

void Board::SetPinMode(int pin, BoardPinMode mode)
{
}

unsigned long Board::TimeMicros()
{
	return 0;
}

unsigned long Board::TimeMillis()
{
	return 0;
}

void Board::DelayMicros(unsigned long micros)
{
}

void Board::DelayMillis(unsigned long millis)
{
}

int Board::EEPROMRead(int address, void* destination, int size)
{
	if (size <= 0)
		return 0;

	int index16 = address / 2;
	int bytesLeft = size;
	if (address & 1)
	{
		*(char*)destination = (char)(eeprom[index16] >> 8);
		destination = (char*)destination + 1;
		index16++;
		bytesLeft--;
	}

	while (bytesLeft > 1)
	{
		*(short*)destination = (short)eeprom[index16];
		destination = (char*)destination + 2;
		index16++;
		bytesLeft -= 2;
	}

	if (bytesLeft > 0)
		*(char*)destination = (char)eeprom[index16];


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
		value = eeprom[index16];
		short newValue = (value & 0x00FF) | ((*(char*)source) << 8);
		if (value != newValue)
			eeprom[index16] = newValue;
		source = (char*)source + 1;
		index16++;
		bytesLeft--;
	}

	while (bytesLeft > 1)
	{
		eeprom[index16] = *(short*)source;
		source = (char*)source + 2;
		index16++;
		bytesLeft -= 2;
	}

	if (bytesLeft > 0)
	{
		value = eeprom[index16];
		short newValue = (value & 0xFF00) | (*(char*)source);
		if (value != newValue)
			eeprom[index16] = newValue;
	}

	return size;
}

void Board::EEPROMCommit()
{

}

void* Board::SerialBegin(int serialIndex, int baudRate)
{
	return null;
}

int Board::SerialBytesAvailable(void* serial)
{
	return 0;
}

int Board::SerialRead(void* serial, const void* destination, int count)
{
	return 0;
}

int Board::SerialWrite(void* serial, const void* source, int count)
{
	for (int i = 0; i < count; i++)
		std::cout << ((char*)source)[i];

	return count;
}

void Board::SerialFlush(void* serial)
{
}


void* Board::I2CBegin(int i2cIndex, unsigned char address)
{
	return null;
}

void Board::I2CEnd(void* i2c)
{
}

int Board::I2CBytesAvailable(void* i2c)
{
	return 0;
}

int Board::I2CRead(void* i2c, const void* destination, int count)
{
	return 0;
}

int Board::I2CWrite(void* i2c, const void* source, int count)
{
	return 0;
}

int Board::I2CRequestData(void* i2c, unsigned char address, int count)
{
	return 0;
}

void Board::I2CBeginWrite(void* i2c, unsigned char address)
{
}

void Board::I2CEndWrite(void* i2c)
{
}

void* Board::ServoBegin(int pin)
{
	return null;
}

void Board::ServoWrite(void* servo, int microseconds)
{
}

void Board::AttachPinInterrupt(int pin, BoardInterruptMode mode, BoadInterruptHandler handler)
{
}

void Board::DetachPinInterrupt(int pin)
{
}

void Board::Pulse(int pin, int timeMicros, int carrierFrequencyHz)
{
}

int Board::AwaitInput(int pin, bool value, int timeMicros)
{
	return 0;
}

#endif