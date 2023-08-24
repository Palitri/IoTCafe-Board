#include "InterruptVector.h"

//BoadInterruptHandler *InterruptVector::slots[maxSlotsCount];

//InterruptVector::slots[0] = InterruptVector::Slot0;

InterruptVector* InterruptVector::vectors[maxSlotsCount];

bool InterruptVector::AttachVector(InterruptVector* vector, int pin, BoardInterruptMode mode)
{
	for (int i = 0; i < InterruptVector::maxSlotsCount; i++)
	{
		if (InterruptVector::vectors[i] == null)
		{
			InterruptVector::vectors[i] = vector;

			vector->interruptPin = pin;

			Board::AttachPinInterrupt(pin, mode, InterruptVector::InterruptSlot0);

			return true;
		}
	}

	return false;
}

bool InterruptVector::DetachVector(InterruptVector* vector)
{
	for (int i = 0; i < InterruptVector::maxSlotsCount; i++)
	{
		if (InterruptVector::vectors[i] == vector)
		{
			Board::DetachPinInterrupt(vector->interruptPin);

			vector->interruptPin = -1;

			InterruptVector::vectors[i] = null;

			return true;
		}
	}

	return false;
}


void InterruptVector::InterruptSlot0()
{
	InterruptVector::vectors[0]->OnInterrupt(InterruptVector::vectors[0]);
}

void InterruptVector::InterruptSlot1()
{
	InterruptVector::vectors[1]->OnInterrupt(InterruptVector::vectors[1]);
}

void InterruptVector::InterruptSlot2()
{
	InterruptVector::vectors[2]->OnInterrupt(InterruptVector::vectors[2]);
}

void InterruptVector::InterruptSlot3()
{
	InterruptVector::vectors[3]->OnInterrupt(InterruptVector::vectors[3]);
}

void InterruptVector::InterruptSlot4()
{
	InterruptVector::vectors[4]->OnInterrupt(InterruptVector::vectors[4]);
}

void InterruptVector::InterruptSlot5()
{
	InterruptVector::vectors[5]->OnInterrupt(InterruptVector::vectors[5]);
}

void InterruptVector::InterruptSlot6()
{
	InterruptVector::vectors[6]->OnInterrupt(InterruptVector::vectors[6]);
}

void InterruptVector::InterruptSlot7()
{
	InterruptVector::vectors[7]->OnInterrupt(InterruptVector::vectors[7]);
}
