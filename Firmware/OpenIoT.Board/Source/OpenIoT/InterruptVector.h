#ifndef InterruptVector_h
#define InterruptVector_h

#include "Board.h"


class InterruptVector
{
private:
	static const int maxSlotsCount = 8;

	static void InterruptSlot0();
	static void InterruptSlot1();
	static void InterruptSlot2();
	static void InterruptSlot3();
	static void InterruptSlot4();
	static void InterruptSlot5();
	static void InterruptSlot6();
	static void InterruptSlot7();

	static InterruptVector* vectors[maxSlotsCount];

public:
	//static BoadInterruptHandler *slots[maxSlotsCount];
	static bool AttachVector(InterruptVector* vector, int pin, BoardInterruptMode mode);
	static bool DetachVector(InterruptVector* vector);

	int interruptPin;

	virtual void OnInterrupt(InterruptVector* vector) = 0;

};

#endif
// InterruptVector_h
