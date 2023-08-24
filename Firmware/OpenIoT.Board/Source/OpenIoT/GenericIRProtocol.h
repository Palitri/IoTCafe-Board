#ifndef GenericIRProtocol_h
#define GenericIRProtocol_h

#include "IRProtocol.h"

#include "Board.h";

class GenericIRProtocol :
	public IRProtocol
{
private:
	static const unsigned char configBit_InitialBurstHigh = 0;
	static const unsigned char configBit_ZeroHigh = 1;
	static const unsigned char configBit_OneHigh = 2;
	static const unsigned char configBit_FinalHigh = 3;
	static const unsigned char configBit_RepeatInitialHigh = 4;
	static const unsigned char configBit_RepeatOneHigh = 5;
	static const unsigned char configBit_DataBitsCount = 6;

	static const unsigned char configBit_InitialBurstLow = 7;
	static const unsigned char configBit_ZeroLow = 8;
	static const unsigned char configBit_OneLow = 9;
	static const unsigned char configBit_RepeatInitialLow = 10;
	static const unsigned char configBit_RepeatInterval = 11;
	static const unsigned char configBit_RepeatsFullData = 12;
	static const unsigned char configBit_InversedDataPulse = 13;

	static const unsigned char configBit_CarrierFrequency = 14;

	static const int carrierFrequenyRC5 = 36000;
	static const int carrierFrequenyNEC = 38000;
	static const int carrierFrequenySony = 40000;

	static const int configDefaultCarrierFrequeny = carrierFrequenyNEC;
	static const unsigned short configDefaultBurstHighMicros = 9000;

	void Burst(int pin, int timeMicros, bool high);

	unsigned short initialBurstHighMicros;
	unsigned short initialBurstLowMicros;
	unsigned short zeroHighMicros;
	unsigned short zeroLowMicros;
	unsigned short oneHighMicros;
	unsigned short oneLowMicros;
	unsigned short finalHighMicros;
	unsigned short dataBitsCount;

	unsigned short repeatInitialHighMicros;
	unsigned short repeatInitialLowMicros;
	unsigned short repeatOneHighMicros;

	bool repeatsFullData;
	bool inversedDataPulse;

	int repeatIntervalMicros, effectiveRepeatIntervalMicros;

	int carrierFrequencyHz, carrierFrequencyHalfTimeMicros;

public:
	static const unsigned char protocolId = 1;

	GenericIRProtocol(void* data = null);
	virtual ~GenericIRProtocol();

	int Configure(void* data);
	virtual void Encode(int pin, void* data, unsigned char dataLength);
	virtual void Repeat(int pin, void* data, unsigned char dataLength);
	virtual int GetRepeatTimeMicros();

};

#endif
// GenericIRProtocol_h
