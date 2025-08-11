//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Math.h"

#include <math.h>
#include <stdlib.h>

const float Math::Pi = 3.14159265358979f;

const float Math::Pi2 = Math::Pi * 2.0f;

const float Math::PiHalf = Math::Pi / 2.0f;

const float Math::Tau = Math::Pi2;

// The golden ratio
const float Math::Phi = 1.61803398874989f;

// Euler's number
const float Math::e = 2.71828182845905;

int Math::Min(int a, int b)
{
	if (a < b)
		return a;

	return b;
}

float Math::Min(float a, float b)
{
	if (a < b)
		return a;

	return b;
}

int Math::Max(int a, int b)
{
	if (a > b)
		return a;

	return b;
}

float Math::Max(float a, float b)
{
	if (a > b)
		return a;

	return b;
}

int Math::Trim(int x, int min, int max)
{
	if (x < min)
		return min;

	if (x > max)
		return max;

	return x;
}

float Math::Trim(float x, float min, float max)
{
	if (x < min)
		return min;

	if (x > max)
		return max;

	return x;
}

int Math::Sign(int x)
{
	return x > 0 ? 1 : x < 0 ? -1 : 0;
}

float Math::Sign(float x)
{
	return x > 0 ? 1.0f : x < 0 ? -1.0f : 0.0f;
}

int Math::Abs(int x)
{
	return x >= 0 ? x : -x;
}

float Math::Abs(float x)
{
	return x >= 0 ? x : -x;
}

float Math::Round(float a)
{
	return round(a);
}

int Math::Power(int base, int exponent)
{
	return (int)pow((float)base, exponent);
}

float Math::Power(float base, float exponent)
{
	return powf(base, exponent);
}

float Math::Sqrt(float x)
{
	return sqrtf(x);
}

float Math::Sin(float x)
{
	return sinf(x);
}

float Math::Cos(float x)
{
	return cosf(x);
}

float Math::Tan(float x)
{
	return tanf(x);
}

float Math::Cot(float x)
{
	return 1.0f / tanf(x);
}

float Math::ArcSin(float x)
{
	return asinf(x);
}

float Math::ArcCos(float x)
{
	return acosf(x);
}

float Math::ArcTan(float x)
{
	return atanf(x);
}

float Math::ArcCot(float x)
{
	return Math::Pi / 2.0f - atanf(x);
}

float Math::ArcTan2(float y, float x)
{
	return atan2f(y, x);
}

int Math::Random(int max)
{
	return rand() % (max + 1);
}

float Math::Exponent(float x, float surface)
{
	return powf(x, 1.0f / surface - 1.0f);
}


unsigned short Math::CRC16(const void* source, unsigned int size, unsigned short seed)
{
	return Math::CRC16Calculate(source, size, Math::CRC16Init(seed));

	//  unsigned short result = seed;
	//  
	//  for (unsigned int i = 0; i < size; i++)
	//  {
	//    unsigned char x = (result >> 8) ^ ((unsigned char*)source)[i];
	//    x ^= x >> 4;
	//    result = (result << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
	//  }
	//
	//  return result; 
}

unsigned short Math::CRC16Init(unsigned short seed)
{
	return seed;
}

unsigned short Math::CRC16Calculate(const void* source, unsigned int size, unsigned short crc)
{
	for (unsigned int i = 0; i < size; i++)
	{
		unsigned char x = crc >> 8 ^ ((unsigned char*)source)[i];
		x ^= x >> 4;
		crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
	}

	return crc;
}
