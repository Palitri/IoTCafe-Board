#include "CodeProcessor.h"

#include "Mem.h"

void CodeProcessor::Operation_NOP(CodeProcessor* context)
{
}

void CodeProcessor::Operation_JMP(CodeProcessor* context)
{
	int jumpOffset = context->ReadInt32();
	context->memoryPosition += jumpOffset;
}

void CodeProcessor::Operation_JZ(CodeProcessor* context)
{
	int condition = context->PopInt();
	int jumpOffset = context->ReadInt32();
	if (condition == 0)
		context->memoryPosition += jumpOffset;
}

void CodeProcessor::Operation_JNZ(CodeProcessor* context)
{
	int condition = context->PopInt();
	int jumpOffset = context->ReadInt32();
	if (condition != 0)
		context->memoryPosition += jumpOffset;
}

void CodeProcessor::Operation_Call(CodeProcessor* context)
{
	int currentAddress = (int)context->memoryPosition;
	int jumpOffset = context->ReadInt32();
	context->PushInt(currentAddress);
	context->memoryPosition += jumpOffset;
}

void CodeProcessor::Operation_CallOut(CodeProcessor* context)
{
	//int calloutId = context->ReadInt32();
	//int calloutId = context->PopInt();
	int calloutId = context->PopUInt8();
	context->OnCallout(calloutId);
}

void CodeProcessor::Operation_RET(CodeProcessor* context)
{
	int returnOffset = context->PopInt();
	context->memoryPosition += returnOffset;
}

// Integer operations

void CodeProcessor::Operation_IToF(CodeProcessor* context)
{
	context->PushFloat((float)context->PopInt());
}

void CodeProcessor::Operation_IPushAddress(CodeProcessor* context)
{
	context->PushInt(context->ReadInt32());
}

void CodeProcessor::Operation_IU8PushAddress(CodeProcessor* context)
{
	context->PushUInt8(context->ReadUInt8());
}

void CodeProcessor::Operation_IPushIndexedAddress(CodeProcessor* context)
{
	int offset = context->PopInt();
	context->PushInt(context->ReadInt32() + offset * 4);
}

void CodeProcessor::Operation_IPushAddressValue(CodeProcessor* context)
{
	context->PushInt(context->ReadAddressInt32Value(context->ReadInt32()));
}

void CodeProcessor::Operation_IPushIndexedAddressValue(CodeProcessor* context)
{
	int offset = context->PopInt();
	context->PushInt(context->ReadAddressInt32Value(context->ReadInt32() + offset * 4));
}


// Evaluation operations

void CodeProcessor::Operation_INot(CodeProcessor* context)
{
	int a = context->PopInt();
	context->PushInt(~a);
}

void CodeProcessor::Operation_IAnd(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a & b);
}

void CodeProcessor::Operation_IOr(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a | b);
}

void CodeProcessor::Operation_IXor(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a ^ b);
}

void CodeProcessor::Operation_IShl(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a << b);
}

void CodeProcessor::Operation_IShr(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a >> b);
}

void CodeProcessor::Operation_IDiv(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a / b);
}

void CodeProcessor::Operation_IMod(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a % b);
}

void CodeProcessor::Operation_IMul(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a * b);
}

void CodeProcessor::Operation_ISub(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a - b);
}

void CodeProcessor::Operation_IAdd(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(a + b);
}

void CodeProcessor::Operation_ILogicalNot(CodeProcessor* context)
{
	int a = context->PopInt();
	context->PushInt(a == 0 ? 1 : 0);
}

void CodeProcessor::Operation_ILogicalAnd(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt(((a != 0) && (b != 0)) ? 1 : 0);
}

void CodeProcessor::Operation_ILogicalOr(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt((a | b) != 0 ? 1 : 0);
}

void CodeProcessor::Operation_IGreater(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt((a > b) ? 1 : 0);
}

void CodeProcessor::Operation_ILesser(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt((a < b) ? 1 : 0);
}

void CodeProcessor::Operation_IEqual(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt((a == b) ? 1 : 0);
}

void CodeProcessor::Operation_INotEqual(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt((a != b) ? 1 : 0);
}

void CodeProcessor::Operation_IGreaterEqual(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt((a >= b) ? 1 : 0);
}

void CodeProcessor::Operation_ILesserEqual(CodeProcessor* context)
{
	int a = context->PopInt();
	int b = context->PopInt();
	context->PushInt((a <= b) ? 1 : 0);
}

// Assignment operations

void CodeProcessor::Operation_ISet(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, value);
}

void CodeProcessor::Operation_IAddAdd(CodeProcessor* context)
{
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) + 1);
}

void CodeProcessor::Operation_ISubSub(CodeProcessor* context)
{
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) - 1);
}

void CodeProcessor::Operation_IAddEquals(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) + value);
}

void CodeProcessor::Operation_ISubEquals(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) - value);
}

void CodeProcessor::Operation_IMulEquals(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) * value);
}

void CodeProcessor::Operation_IModEquals(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) % value);
}

void CodeProcessor::Operation_IDivEquals(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) / value);
}

void CodeProcessor::Operation_IXorEquals(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) ^ value);
}

void CodeProcessor::Operation_IOrEquals(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) | value);
}

void CodeProcessor::Operation_IAndEquals(CodeProcessor* context)
{
	int value = context->PopInt();
	int address = context->PopInt();
	context->WriteAddressInt32Value(address, context->ReadAddressInt32Value(address) & value);
}

// Floating point operaions

void CodeProcessor::Operation_FToI(CodeProcessor* context)
{
	context->PushInt((int)context->PopFloat());
}

void CodeProcessor::Operation_FPushAddress(CodeProcessor* context)
{
	context->PushInt(context->ReadInt32());
}

void CodeProcessor::Operation_FPushIndexedAddress(CodeProcessor* context)
{
	int offset = context->PopInt();
	context->PushInt(context->ReadInt32() + offset * 4);
}

void CodeProcessor::Operation_FPushAddressValue(CodeProcessor* context)
{
	context->PushFloat(context->ReadAddressFloat32Value(context->ReadInt32()));
}

void CodeProcessor::Operation_FPushIndexedAddressValue(CodeProcessor* context)
{
	int offset = (int)context->PopInt();
	context->PushFloat(context->ReadAddressFloat32Value(context->ReadInt32() + offset * 4));
}

// Evaluation operations

void CodeProcessor::Operation_FDiv(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushFloat(a / b);
}

void CodeProcessor::Operation_FMul(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushFloat(a * b);
}

void CodeProcessor::Operation_FSub(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushFloat(a - b);
}

void CodeProcessor::Operation_FAdd(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushFloat(a + b);
}

void CodeProcessor::Operation_FGreater(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushInt((a > b) ? 1 : 0);
}

void CodeProcessor::Operation_FLesser(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushInt((a < b) ? 1 : 0);
}

void CodeProcessor::Operation_FEqual(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushInt((a == b) ? 1 : 0);
}

void CodeProcessor::Operation_FNotEqual(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushInt((a != b) ? 1 : 0);
}

void CodeProcessor::Operation_FGreaterEqual(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushInt((a >= b) ? 1 : 0);
}

void CodeProcessor::Operation_FLesserEqual(CodeProcessor* context)
{
	float a = context->PopFloat();
	float b = context->PopFloat();
	context->PushInt((a <= b) ? 1 : 0);
}

// Assignment operations

void CodeProcessor::Operation_FSet(CodeProcessor* context)
{
	float value = context->PopFloat();
	int address = context->PopInt();
	context->WriteAddressFloat32Value(address, value);
}

void CodeProcessor::Operation_FAddEquals(CodeProcessor* context)
{
	float value = context->PopFloat();
	int address = context->PopInt();
	context->WriteAddressFloat32Value(address, context->ReadAddressFloat32Value(address) + value);
}

void CodeProcessor::Operation_FSubEquals(CodeProcessor* context)
{
	float value = context->PopFloat();
	int address = context->PopInt();
	context->WriteAddressFloat32Value(address, context->ReadAddressFloat32Value(address) - value);
}

void CodeProcessor::Operation_FMulEquals(CodeProcessor* context)
{
	float value = context->PopFloat();
	int address = context->PopInt();
	context->WriteAddressFloat32Value(address, context->ReadAddressFloat32Value(address) * value);
}

void CodeProcessor::Operation_FDivEquals(CodeProcessor* context)
{
	float value = context->PopFloat();
	int address = context->PopInt();
	context->WriteAddressFloat32Value(address, context->ReadAddressFloat32Value(address) / value);
}

CodeProcessor::CodeProcessor(void)
{
	this->AddOperation(CodeProcessor::Operation_NOP);
	
	this->operationId_NOP = this->AddOperation(CodeProcessor::Operation_NOP);
	this->operationId_JMP = this->AddOperation(CodeProcessor::Operation_JMP);
	this->operationId_JZ = this->AddOperation(CodeProcessor::Operation_JZ);
	this->operationId_JNZ = this->AddOperation(CodeProcessor::Operation_JNZ);
	this->operationId_Call = this->AddOperation(CodeProcessor::Operation_Call);
	this->operationId_RET = this->AddOperation(CodeProcessor::Operation_RET);
	this->operationId_CallOut = this->AddOperation(CodeProcessor::Operation_CallOut);

	// Integer
	this->operationId_IToF = this->AddOperation(CodeProcessor::Operation_IToF);
	this->operationId_IPushAddress = this->AddOperation(CodeProcessor::Operation_IPushAddress);
	this->operationId_IU8PushAddress = this->AddOperation(CodeProcessor::Operation_IU8PushAddress);
	this->operationId_IPushIndexedAddress = this->AddOperation(CodeProcessor::Operation_IPushIndexedAddress);
	this->operationId_IPushAddressValue = this->AddOperation(CodeProcessor::Operation_IPushAddressValue);
	this->operationId_IPushIndexedAddressValue = this->AddOperation(CodeProcessor::Operation_IPushIndexedAddressValue);
	// Evaluation
	this->operationId_INot = this->AddOperation(CodeProcessor::Operation_INot);
	this->operationId_IAnd = this->AddOperation(CodeProcessor::Operation_IAnd);
	this->operationId_IOr = this->AddOperation(CodeProcessor::Operation_IOr);
	this->operationId_IXor = this->AddOperation(CodeProcessor::Operation_IXor);
	this->operationId_IShl = this->AddOperation(CodeProcessor::Operation_IShl);
	this->operationId_IShr = this->AddOperation(CodeProcessor::Operation_IShr);
	this->operationId_IDiv = this->AddOperation(CodeProcessor::Operation_IDiv);
	this->operationId_IMod = this->AddOperation(CodeProcessor::Operation_IMod);
	this->operationId_IMul = this->AddOperation(CodeProcessor::Operation_IMul);
	this->operationId_ISub = this->AddOperation(CodeProcessor::Operation_ISub);
	this->operationId_IAdd = this->AddOperation(CodeProcessor::Operation_IAdd);
	this->operationId_ILogicalNot = this->AddOperation(CodeProcessor::Operation_ILogicalNot);
	this->operationId_ILogicalAnd = this->AddOperation(CodeProcessor::Operation_ILogicalAnd);
	this->operationId_ILogicalOr = this->AddOperation(CodeProcessor::Operation_ILogicalOr);
	this->operationId_IGreater = this->AddOperation(CodeProcessor::Operation_IGreater);
	this->operationId_ILesser = this->AddOperation(CodeProcessor::Operation_ILesser);
	this->operationId_IEqual = this->AddOperation(CodeProcessor::Operation_IEqual);
	this->operationId_INotEqual = this->AddOperation(CodeProcessor::Operation_INotEqual);
	this->operationId_IGreaterEqual = this->AddOperation(CodeProcessor::Operation_IGreaterEqual);
	this->operationId_ILesserEqual = this->AddOperation(CodeProcessor::Operation_ILesserEqual);
	// Assignment	
	this->operationId_ISet = this->AddOperation(CodeProcessor::Operation_ISet);
	this->operationId_IAddAdd = this->AddOperation(CodeProcessor::Operation_IAddAdd);
	this->operationId_ISubSub = this->AddOperation(CodeProcessor::Operation_ISubSub);
	this->operationId_IAddEquals = this->AddOperation(CodeProcessor::Operation_IAddEquals);
	this->operationId_ISubEquals = this->AddOperation(CodeProcessor::Operation_ISubEquals);
	this->operationId_IMulEquals = this->AddOperation(CodeProcessor::Operation_IMulEquals);
	this->operationId_IModEquals = this->AddOperation(CodeProcessor::Operation_IModEquals);
	this->operationId_IDivEquals = this->AddOperation(CodeProcessor::Operation_IDivEquals);
	this->operationId_IXorEquals = this->AddOperation(CodeProcessor::Operation_IXorEquals);
	this->operationId_IOrEquals = this->AddOperation(CodeProcessor::Operation_IOrEquals);
	this->operationId_IAndEquals = this->AddOperation(CodeProcessor::Operation_IAndEquals);

	// Floating point
	this->operationId_FToI = this->AddOperation(CodeProcessor::Operation_FToI);
	this->operationId_FPushAddress = this->AddOperation(CodeProcessor::Operation_FPushAddress);
	this->operationId_FPushIndexedAddress = this->AddOperation(CodeProcessor::Operation_FPushIndexedAddress);
	this->operationId_FPushAddressValue = this->AddOperation(CodeProcessor::Operation_FPushAddressValue);
	this->operationId_FPushIndexedAddressValue = this->AddOperation(CodeProcessor::Operation_FPushIndexedAddressValue);
	// Evaluation
	this->operationId_FDiv = this->AddOperation(CodeProcessor::Operation_FDiv);
	this->operationId_FMul = this->AddOperation(CodeProcessor::Operation_FMul);
	this->operationId_FSub = this->AddOperation(CodeProcessor::Operation_FSub);
	this->operationId_FAdd = this->AddOperation(CodeProcessor::Operation_FAdd);
	this->operationId_FGreater = this->AddOperation(CodeProcessor::Operation_FGreater);
	this->operationId_FLesser = this->AddOperation(CodeProcessor::Operation_FLesser);
	this->operationId_FEqual = this->AddOperation(CodeProcessor::Operation_FEqual);
	this->operationId_FNotEqual = this->AddOperation(CodeProcessor::Operation_FNotEqual);
	this->operationId_FGreaterEqual = this->AddOperation(CodeProcessor::Operation_FGreaterEqual);
	this->operationId_FLesserEqual = this->AddOperation(CodeProcessor::Operation_FLesserEqual);
	// Assignment	
	this->operationId_FSet = this->AddOperation(CodeProcessor::Operation_FSet);
	this->operationId_FAddEquals = this->AddOperation(CodeProcessor::Operation_FAddEquals);
	this->operationId_FSubEquals = this->AddOperation(CodeProcessor::Operation_FSubEquals);
	this->operationId_FMulEquals = this->AddOperation(CodeProcessor::Operation_FMulEquals);
	this->operationId_FDivEquals = this->AddOperation(CodeProcessor::Operation_FDivEquals);

	this->stackCapacity = 0;
	this->stackPos = 0;
	this->stack = 0;
	this->EnsureStackCapacity(256);

	this->memoryPosition = 0;
	this->memoryLength = 0;
	this->memory = 0;
}


CodeProcessor::~CodeProcessor(void)
{
	Mem::FreeAndNull(&this->stack);
}

void CodeProcessor::EnsureStackCapacity(int capacity)
{
	if (this->stackCapacity >= capacity)
		return;

	if (this->stackCapacity <= 0)
		this->stackCapacity = 1;

	while (this->stackCapacity < capacity)
		this->stackCapacity *= 2;

	void* newStack = Mem::Get(capacity);
	Mem::Copy(newStack, this->stack, this->stackPos);
	Mem::FreeAndNull(&this->stack);
	this->stack = newStack;
	this->stackCapacity = capacity;
}

unsigned char CodeProcessor::ReadUInt8()
{
	unsigned char result = *(unsigned char*)((int)this->memory + this->memoryPosition);
	this->memoryPosition += 1;
	return result;
}

int CodeProcessor::ReadInt32()
{
	int result = *(int*)((int)this->memory + this->memoryPosition);
	this->memoryPosition += 4;
	return result;
}

float CodeProcessor::ReadFloat()
{
	float result = *(float*)((int)this->memory + this->memoryPosition);
	this->memoryPosition += 4;
	return result;
}

void CodeProcessor::PushInt(int value)
{
	this->EnsureStackCapacity(this->stackPos + 4);

	*(int*)((int)this->stack + this->stackPos) = value;
	this->stackPos += 4;
}

int CodeProcessor::PopInt()
{
	this->stackPos -= 4;
	int result = *(int*)((int)this->stack + this->stackPos);
	return result;
}


void CodeProcessor::PushUInt8(unsigned char value)
{
	this->EnsureStackCapacity(this->stackPos + 1);

	*(unsigned char*)((int)this->stack + this->stackPos) = value;
	this->stackPos += 1;
}

unsigned char CodeProcessor::PopUInt8()
{
	this->stackPos -= 1;
	unsigned char result = *(unsigned char*)((int)this->stack + this->stackPos);
	return result;
}


void CodeProcessor::PushFloat(float value)
{
	this->EnsureStackCapacity(this->stackPos + 4);

	*(float*)((int)this->stack + this->stackPos) = value;
	this->stackPos += 4;
}

float CodeProcessor::PopFloat()
{
	this->stackPos -= 4;
	float result = *(float*)((int)this->stack + this->stackPos);
	return result;
}

unsigned char CodeProcessor::ReadAddressUInt8Value(unsigned char address)
{
	return *(unsigned char*)((unsigned int)this->memory + address);
}

void CodeProcessor::WriteAddressUInt8Value(int address, unsigned char value)
{
	*(unsigned char*)((unsigned int)this->memory + address) = value;
}

int CodeProcessor::ReadAddressInt32Value(int address)
{
	return *(int*)((unsigned int)this->memory + address);
}

void CodeProcessor::WriteAddressInt32Value(int address, int value)
{
	*(int*)((unsigned int)this->memory + address) = value;
}

float CodeProcessor::ReadAddressFloat32Value(int address)
{
	return *(float*)((unsigned int)this->memory + address);
}

void CodeProcessor::WriteAddressFloat32Value(int address, float value)
{
	*(float*)((unsigned int)this->memory + address) = value;
}

int CodeProcessor::ReadAddress7Bit32Value(int address)
{
	unsigned char* memory = (unsigned char*)((unsigned int)this->memory + address);
	unsigned char bits = *memory;
	int result = bits;
	while ((bits & 0x80) != 0)
	{
		memory++;
		bits = *memory;
		result = (result << 7) | (bits & 0x7f);
	}

	return result;
}

void CodeProcessor::WriteAddress7Bit32Value(int address, int value)
{
	unsigned char* memory = (unsigned char*)((unsigned int)this->memory + address);
	*memory = (value & 0x7f);
	value = value >> 7;

	while (value != 0)
	{
		*memory |= 0x80;
		memory++;
		*memory = (value & 0x7f);
		value = value >> 7;
	}
}


int CodeProcessor::AddOperation(CodeProcessorOperation operation)
{
	this->operations.Add(operation);
	return this->operations.count - 1;
}

void CodeProcessor::SetMemory(void* memory, unsigned int memoryLength)
{
	this->memory = memory;
	this->memoryPosition = 0;
	this->memoryLength = memoryLength;
}

void CodeProcessor::Process(bool fromStart)
{
	if (fromStart)
		this->memoryPosition = 0;

	while (this->memoryPosition < this->memoryLength)
	{
		unsigned char operationId = *(unsigned char*)((unsigned int)this->memory + this->memoryPosition);
		this->memoryPosition++;

		this->operations[operationId](this);
	}
}

void CodeProcessor::OnCallout(int calloutId)
{
}
