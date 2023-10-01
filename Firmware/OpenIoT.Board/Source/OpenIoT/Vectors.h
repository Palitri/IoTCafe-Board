//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#ifndef Vectors_h
#define Vectors_h


template <typename T> class Vector2Generic
{
public:

	T x, y;

	//  void operator = (const T &argument)
	//  {
	//    this->x = argument;
	//    this->y = argument;
	//  };
	//
	//  void operator = (const Vector2Generic<T> &argument)
	//  {
	//    this->x = argument.x;
	//    this->y = argument.y;
	//  };
	//
	//  Vector2Generic<T> operator + (const Vector2Generic<T> &argument)
	//  {
	//    return Vector2Generic<T>(this->x + argument.x, this->y + argument.y);
	//  };
	//
	//  Vector2Generic<T> operator - (const Vector2Generic<T> &argument)
	//  {
	//    return Vector2Generic<T>(this->x - argument.x, this->y - argument.y);
	//  };
	//  
	//  Vector2Generic<T> operator * (const T &argument)
	//  {
	//    return Vector2Generic<T>(this->x * argument, this->y * argument);
	//  };
	//
	//  Vector2Generic<T> operator / (const T &argument)
	//  {
	//    return Vector2Generic<T>(this->x / argument, this->y / argument);
	//  };

	bool Equals(Vector2Generic<T>* v)
	{
		return (this->x == v->x) && (this->y == v->y);
	};

	void Set(T x, T y)
	{
		this->x = x;
		this->y = y;
	};

	void Set(T value)
	{
		this->x = value;
		this->y = value;
	};

	static float Length(Vector2Generic<T>& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	static void Add(Vector2Generic<T>& result, Vector2Generic<T>& v1, Vector2Generic<T>& v2)
	{
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
	};

	static void Subtract(Vector2Generic<T>& result, Vector2Generic<T>& v1, Vector2Generic<T>& v2)
	{
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
	};

	static void Scale(Vector2Generic<T>& result, Vector2Generic<T>& v, T factor)
	{
		result.x = v.x * factor;
		result.y = v.y * factor;
	};

	static void Divide(Vector2Generic<T>& result, Vector2Generic<T>& v, T factor)
	{
		result.x = v.x / factor;
		result.y = v.y / factor;
	};

	static T Normalize(Vector2Generic<T>& result, Vector2Generic<T>& v)
	{
		T length = sqrt(v.x * v.x + v.y * v.y);
		if (length == 0)
			return 0;

		T factor = (T)1 / length;

		result.x = v.x * factor;
		result.y = v.y * factor;

		return length;
	};

	static T SetLength(Vector2Generic<T>& result, Vector2Generic<T>& v, T length)
	{
		T originalLength = sqrt(v.x * v.x + v.y * v.y);
		if (originalLength == 0)
			return 0;

		T factor = length / originalLength;

		result.x = v.x * factor;
		result.y = v.y * factor;

		return originalLength;
	};
};

template <typename T> class Vector3Generic
{
public:

	T x, y, z;

	//  virtual void operator = (const T &argument)
	//  {
	//    this->x = argument;
	//    this->y = argument;
	//    this->z = argument;
	//  };
	//
	//  virtual void operator = (const Vector3Generic<T> &argument)
	//  {
	//    this->x = argument.x;
	//    this->y = argument.y;
	//    this->z = argument.z;
	//  };
	//
	//  virtual Vector3Generic<T> operator + (const Vector3Generic<T> &argument)
	//  {
	//    return Vector3Generic<T>(this->x + argument.x, this->y + argument.y, this->z + argument.z);
	//  };
	//
	//  Vector3Generic<T> operator - (const Vector3Generic<T> &argument)
	//  {
	//    return Vector3Generic<T>(this->x - argument.x, this->y - argument.y, this->z - argument.z);
	//  };
	//  
	//  Vector3Generic<T> operator * (const T &argument)
	//  {
	//    return Vector3Generic<T>(this->x * argument, this->y * argument, this->z * argument);
	//  };
	//
	//  Vector3Generic<T> operator / (const T &argument)
	//  {
	//    return Vector3Generic<T>(this->x / argument, this->y / argument, this->z / argument);
	//  };

	bool Equals(Vector3Generic<T>* v)
	{
		return (this->x == v->x) && (this->y == v->y) && (this->z == v->z);
	};

	void Set(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	};

	void Set(T value)
	{
		this->x = value;
		this->y = value;
		this->z = value;
	};

	static float Length(Vector3Generic<T>& v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	static void Add(Vector3Generic<T>& result, Vector3Generic<T>& v1, Vector3Generic<T>& v2)
	{
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
	};

	static void Subtract(Vector3Generic<T>& result, Vector3Generic<T>& v1, Vector3Generic<T>& v2)
	{
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
	};

	static void Scale(Vector3Generic<T>& result, Vector3Generic<T>& v, T factor)
	{
		result.x = v.x * factor;
		result.y = v.y * factor;
		result.z = v.z * factor;
	};

	static void Divide(Vector3Generic<T>& result, Vector3Generic<T>& v, T factor)
	{
		result.x = v.x / factor;
		result.y = v.y / factor;
		result.z = v.z / factor;
	};

	static T Normalize(Vector3Generic<T>& result, Vector3Generic<T>& v)
	{
		T length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		if (length == 0)
			return 0;

		T factor = (T)1 / length;

		result.x = v.x * factor;
		result.y = v.y * factor;
		result.z = v.z * factor;

		return length;
	};

	static T SetLength(Vector3Generic<T>& result, Vector3Generic<T>& v, T length)
	{
		T originalLength = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		if (originalLength == 0)
			return 0;

		T factor = length / originalLength;

		result.x = v.x * factor;
		result.y = v.y * factor;
		result.z = v.z * factor;

		return originalLength;
	};

	static void Invert(Vector3Generic<T>& result, Vector3Generic<T>& v)
	{
		result.x = -v.x;
		result.y = -v.y;
		result.z = -v.z;
	};

	static T Dot(Vector3Generic<T>& v1, Vector3Generic<T>& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}
};

class Vector2
	: public Vector2Generic<float>
{
};

class Vector2I
	: public Vector2Generic<int>
{
};

class Vector3
	: public Vector3Generic<float>
{
public:
	Vector3()
	{
	};

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	};
};

class Vector3I
	: public Vector3Generic<int>
{
};

class Vector3I16
	: public Vector3Generic<short>
{
};

#endif
// Vectors_h

