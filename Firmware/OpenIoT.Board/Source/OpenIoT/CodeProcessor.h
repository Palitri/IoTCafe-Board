#pragma once

#include "List.h"

class CodeProcessor;

typedef void (*CodeProcessorOperation) (CodeProcessor* context);

class CodeProcessor
{
private:
	static void Operation_NOP(CodeProcessor* context);
	static void Operation_JMP(CodeProcessor* context);
	static void Operation_JZ(CodeProcessor* context);
	static void Operation_JNZ(CodeProcessor* context);
	static void Operation_Call(CodeProcessor* context);
	static void Operation_RET(CodeProcessor* context);
	static void Operation_CallOut(CodeProcessor* context);

	// Integer
	static void Operation_IToF(CodeProcessor* context);
	static void Operation_IPushAddress(CodeProcessor* context);
	static void Operation_IU8PushAddress(CodeProcessor* context);
	static void Operation_IPushIndexedAddress(CodeProcessor* context);
	static void Operation_IPushAddressValue(CodeProcessor* context);
	static void Operation_IPushIndexedAddressValue(CodeProcessor* context);
	// Evaluation
	static void Operation_INot(CodeProcessor* context);
	static void Operation_IAnd(CodeProcessor* context);
	static void Operation_IOr(CodeProcessor* context);
	static void Operation_IXor(CodeProcessor* context);
	static void Operation_IShl(CodeProcessor* context);
	static void Operation_IShr(CodeProcessor* context);
	static void Operation_IDiv(CodeProcessor* context);
	static void Operation_IMod(CodeProcessor* context);
	static void Operation_IMul(CodeProcessor* context);
	static void Operation_ISub(CodeProcessor* context);
	static void Operation_IAdd(CodeProcessor* context);
	static void Operation_ILogicalNot(CodeProcessor* context);
	static void Operation_ILogicalAnd(CodeProcessor* context);
	static void Operation_ILogicalOr(CodeProcessor* context);
	static void Operation_IGreater(CodeProcessor* context);
	static void Operation_ILesser(CodeProcessor* context);
	static void Operation_IEqual(CodeProcessor* context);
	static void Operation_INotEqual(CodeProcessor* context);
	static void Operation_IGreaterEqual(CodeProcessor* context);
	static void Operation_ILesserEqual(CodeProcessor* context);
	// Assignment
	static void Operation_ISet(CodeProcessor* context);
	static void Operation_IAddAdd(CodeProcessor* context);
	static void Operation_ISubSub(CodeProcessor* context);
	static void Operation_IAddEquals(CodeProcessor* context);
	static void Operation_ISubEquals(CodeProcessor* context);
	static void Operation_IMulEquals(CodeProcessor* context);
	static void Operation_IModEquals(CodeProcessor* context);
	static void Operation_IDivEquals(CodeProcessor* context);
	static void Operation_IXorEquals(CodeProcessor* context);
	static void Operation_IOrEquals(CodeProcessor* context);
	static void Operation_IAndEquals(CodeProcessor* context);

	// Floating point
	static void Operation_FToI(CodeProcessor* context);
	static void Operation_FPushAddress(CodeProcessor* context);
	static void Operation_FPushIndexedAddress(CodeProcessor* context);
	static void Operation_FPushAddressValue(CodeProcessor* context);
	static void Operation_FPushIndexedAddressValue(CodeProcessor* context);
	// Evaluations
	static void Operation_FGreater(CodeProcessor* context);
	static void Operation_FLesser(CodeProcessor* context);
	static void Operation_FEqual(CodeProcessor* context);
	static void Operation_FNotEqual(CodeProcessor* context);
	static void Operation_FGreaterEqual(CodeProcessor* context);
	static void Operation_FLesserEqual(CodeProcessor* context);
	static void Operation_FDiv(CodeProcessor* context);
	static void Operation_FMul(CodeProcessor* context);
	static void Operation_FSub(CodeProcessor* context);
	static void Operation_FAdd(CodeProcessor* context);
	// Assignment
	static void Operation_FSet(CodeProcessor* context);
	static void Operation_FAddEquals(CodeProcessor* context);
	static void Operation_FSubEquals(CodeProcessor* context);
	static void Operation_FMulEquals(CodeProcessor* context);
	static void Operation_FDivEquals(CodeProcessor* context);


protected:
	void* stack;
	int stackCapacity, stackPos;

	void EnsureStackCapacity(int capacity);

	unsigned char ReadUInt8();
	int ReadInt32();
	float ReadFloat();

	void PushInt(int value);
	int PopInt();

	void PushUInt8(unsigned char value);
	unsigned char PopUInt8();

	void PushFloat(float value);
	float PopFloat();

	unsigned char ReadAddressUInt8Value(unsigned char address);
	void WriteAddressUInt8Value(int address, unsigned char value);

	int ReadAddressInt32Value(int address);
	void WriteAddressInt32Value(int address, int value);

	float ReadAddressFloat32Value(int address);
	void WriteAddressFloat32Value(int address, float value);

	int ReadAddress7Bit32Value(int address);
	void WriteAddress7Bit32Value(int address, int value);

public:
	int
		operationId_NOP,
		operationId_JMP,
		operationId_JZ,
		operationId_JNZ,
		operationId_Call,
		operationId_RET,
		operationId_CallOut,

		operationId_IToF,
		operationId_IPushAddress,
		operationId_IU8PushAddress,
		operationId_IPushIndexedAddress,
		operationId_IPushAddressValue,
		operationId_IPushIndexedAddressValue,

		operationId_INot,
		operationId_IAnd,
		operationId_IOr,
		operationId_IXor,
		operationId_IShl,
		operationId_IShr,
		operationId_IDiv,
		operationId_IMul,
		operationId_IMod,
		operationId_ISub,
		operationId_IAdd,
		operationId_ILogicalNot,
		operationId_ILogicalAnd,
		operationId_ILogicalOr,
		operationId_IGreater,
		operationId_ILesser,
		operationId_IEqual,
		operationId_INotEqual,
		operationId_IGreaterEqual,
		operationId_ILesserEqual,

		operationId_ISet,
		operationId_IAddAdd,
		operationId_ISubSub,
		operationId_IAddEquals,
		operationId_ISubEquals,
		operationId_IModEquals,
		operationId_IMulEquals,
		operationId_IDivEquals,
		operationId_IXorEquals,
		operationId_IOrEquals,
		operationId_IAndEquals,

		operationId_FToI,
		operationId_FPushAddress,
		operationId_FPushIndexedAddress,
		operationId_FPushAddressValue,
		operationId_FPushIndexedAddressValue,

		operationId_FDiv,
		operationId_FMul,
		operationId_FSub,
		operationId_FAdd,
		operationId_FGreater,
		operationId_FLesser,
		operationId_FEqual,
		operationId_FNotEqual,
		operationId_FGreaterEqual,
		operationId_FLesserEqual,

		operationId_FSet,
		operationId_FAddEquals,
		operationId_FSubEquals,
		operationId_FMulEquals,
		operationId_FDivEquals;

	List<CodeProcessorOperation> operations;

	void* memory;
	unsigned int memoryPosition;
	unsigned int memoryLength;


	CodeProcessor(void);
	~CodeProcessor(void);

	// Adds an operation to the processor the returns the id given to that operation
	int AddOperation(CodeProcessorOperation operation);

	// Sets the memory which holds the code to be processed
	void SetMemory(void* memory, unsigned int memoryLength);
	// Runs the code from memory
	void Process(bool fromStart = true);

	// Called in a Callout operation, used for communication between script and native code
	virtual void OnCallout(int calloutId);
};
