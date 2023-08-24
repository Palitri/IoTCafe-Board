#include "PeripheralStepMotor.h"

#include "Board.h"

PeripheralStepMotor::PeripheralStepMotor(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(2);

	this->motor = null;
}

PeripheralStepMotor::~PeripheralStepMotor()
{
	if (this->motor != null)
		delete this->motor;
}


int PeripheralStepMotor::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->pTurns = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->pTurnsPerSec = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));


	unsigned char pinStep = *charCode++;
	unsigned char pinDir = *charCode++;
	short fullStepsPerTurn = *(short*)charCode;
	charCode += 2;

	this->motor = new StepMotorDriver(pinStep, pinDir, 1.0, fullStepsPerTurn);

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralStepMotor::Update()
{
	float pTurns = this->pTurns->fValue;
	float pTurnsPerSec = this->pTurnsPerSec->fValue;

	if (pTurns * pTurnsPerSec != 0.0f)
	{
		this->motor->Begin(0.0f, pTurns);

		unsigned long time = Board::TimeMicros();
		float u = 0.0f;
		do {
			this->motor->Drive(u);
			u = (float)(Board::TimeMicros() - time) / (pTurnsPerSec * 1000000.0f);
		} while (u < pTurnsPerSec);

		this->pTurns->SetFloat(0.0f);
	}

	this->UpdateProperties();
}
