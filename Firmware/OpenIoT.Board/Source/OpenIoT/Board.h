#ifndef Board_h
#define Board_h

#ifdef ARDUINO
	//#define BOARD_STM32F103C

	//#define BOARD_ESP8266
	#define BOARD_ESP32
#else
	#define BOARD_PC
#endif


#define null 0

// TODO: About time to intoduce fixed types, though here's definitely not the best place.
// Solution proposal: Make .h files for each board and #include the corresponding .h in the universal Board.h?? Types would be defined in these board-specific .h files.
//typedef signed char        int8_t;
//typedef short              int16_t;
//typedef int                int32_t;
//typedef long long          int64_t;
//typedef unsigned char      uint8_t;
//typedef unsigned short     uint16_t;
//typedef unsigned int       uint32_t;
//typedef unsigned long long uint64_t;


//#define int8_t		signed char
//#define int16_t		short
//#define int32_t		int
//#define int64_t		long long
//#define uint8_t		unsigned char
//#define uint16_t	unsigned short
//#define uint32_t	unsigned int
//#define uint64_t	unsigned long long



class IClusterDevice;

enum BoardPinMode
{
	BoardPinMode_DigitalInput = 1,
	BoardPinMode_DigitalOutput = 2,
	BoardPinMode_PWMOutput = 3,
	BoardPinMode_AnalogInput = 4,
	BoardPinMode_DigitalInput_Pullup = 5,
};


enum BoardInterruptMode
{
	BoardInterruptMode_Rising = 1,
	BoardInterruptMode_Falling = 2,
	BoardInterruptMode_Change = 3,
	BoardInterruptMode_Low = 4,
	BoardInterruptMode_High = 5
};

typedef void (BoadInterruptHandler)();


class Board
{
private:
	static unsigned char nativeTriggerModesMap[5];

public:
	static const int pwmMaxValue = 0xFFFF;

	static const int analogReadMaxValue = 4095;
	static const int analogReadHalfValue = analogReadMaxValue / 2;

	static const int eepromSize;
	static const char* name;

	static void Initialize(IClusterDevice* clusterDevice);

	static void* DispatchFeaturesObject();
		
	static bool DigitalRead(int pin);
	static void DigitalWrite(int pin, bool on);
	static float AnalogRead(int pin);
	static void SetPwm(int pin, float duty);

	static void SetPinMode(int pin, BoardPinMode mode);

	static unsigned long TimeMicros();
	static unsigned long TimeMillis();
	static void DelayMicros(unsigned long micros);
	static void DelayMillis(unsigned long millis);

	static int EEPROMRead(int address, void *destination, int size);
	static int EEPROMWrite(int address, const void* source, int size, bool force = false);
	static void EEPROMCommit();

	static void* SerialBegin(int serialIndex, int baudRate);
	static int SerialBytesAvailable(void* serial);
	static int SerialRead(void* serial, const void* destination, int count);
	static int SerialWrite(void* serial, const void* source, int count);
	static void SerialFlush(void* serial);

	static void* I2CBegin(int i2cIndex, unsigned char address = 0xFF);
	static void I2CEnd(void* i2c);
	static int I2CBytesAvailable(void* i2c);
	static int I2CReadMsbFirst(void* i2c, const void* destination, int count);
	static int I2CReadLsbFirst(void* i2c, const void* destination, int count);
	static int I2CWrite(void* i2c, const void* source, int count);
	static int I2CRequestData(void* i2c, unsigned char address, int count);
	static void I2CBeginWrite(void* i2c, unsigned char address);
	static void I2CEndWrite(void* i2c);

	static void* ServoBegin(int pin);
	static void ServoWrite(void* servo, int microseconds);

	static void AttachPinInterrupt(int pin, BoardInterruptMode mode, BoadInterruptHandler handler);
	static void DetachPinInterrupt(int pin);

	static void Pulse(int pin, int timeMicros, int carrierFrequencyHz);

	static int AwaitInput(int pin, bool value, int timeMicros);
};

#endif
// Board_h
