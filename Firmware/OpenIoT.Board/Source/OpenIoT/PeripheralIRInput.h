#ifndef PeripheralIRInput_h
#define PeripheralIRInput_h

#include "Peripheral.h"

#include "Property.h"

class PeripheralIRInput :
	public Peripheral
{
private:
	static const int defaultInitialBurstHighMicros = 9000;
	static const int defaultBitBurstHighMicros = defaultInitialBurstHighMicros / 16;

	static const int minIdleTimeBetweenPacketsMicros = 100000;

	static const int maxBitBytes = 16;
	static const int maxBits = maxBitBytes * 8;

	static PeripheralIRInput* interruptInstance;
	static void InterruptHandler();

	void Reset();

	unsigned char pin;

	volatile int bitIndex;
	volatile unsigned char bits[maxBitBytes];
	volatile unsigned short bitsHash;

	volatile unsigned long timeLastRead;
	volatile unsigned long timeZeroBitPulse, timeInitialSequence;

	volatile bool sequenceReceived;

	volatile unsigned long timeMaxZeroBitPulse;

	void OnInterrupt();
	unsigned short GetBitsHash();
	void CreateIRCode();

public:
	static const int Code_DeviceId = 24;

	Property* value, * code, * configuration;

	PeripheralIRInput(IClusterDevice* device = null);
	virtual ~PeripheralIRInput();

	virtual int Load(const void* code);
	virtual void Update();
};

#endif
// PeripheralIRInput_h
