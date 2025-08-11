#ifndef Math_h
#define Math_h

class Math
{
public:
	static const float Pi;
	static const float Pi2;
	static const float PiHalf;
	static const float Tau;
	static const float Phi; // The golden ratio
	static const float e; // Euler's number


	static int Min(int a, int b);
	static float Min(float a, float b);
	static int Max(int a, int b);
	static float Max(float a, float b);
	static int Trim(int x, int min, int max);
	static float Trim(float x, float min, float max);
	static int Sign(int x);
	static float Sign(float x);
	static int Abs(int x);
	static float Abs(float x);
	static float Round(float a);
	static int Power(int base, int exponent);
	static float Power(float base, float exponent);
	static float Sqrt(float x);
	static float Sin(float x);
	static float Cos(float x);
	static float Tan(float x);
	static float Cot(float x);
	static float ArcSin(float x);
	static float ArcCos(float x);
	static float ArcTan(float x);
	static float ArcCot(float x);
	static float ArcTan2(float y, float x);
	static int Random(int max);
	static float Exponent(float x, float surface);

	static unsigned short CRC16(const void* source, unsigned int size, unsigned short seed = 0x1D0F);

	static unsigned short CRC16Init(unsigned short seed = 0x1D0F);
	static unsigned short CRC16Calculate(const void* source, unsigned int size, unsigned short crc);
};

#endif
// Math_h

