#ifndef Matrix3_h
#define Matrix3_h

class Matrix3
{
public:
	float
		m11, m12, m13,
		m21, m22, m23,
		m31, m32, m33;

	static float GetDeterminant(Matrix3& matrix);
	static void CreateIdentity(Matrix3& result);
	static void CreateRotationX(Matrix3& result, float phi);
	static void CreateRotationY(Matrix3& result, float phi);
	static void CreateRotationZ(Matrix3& result, float phi);
	static void CreateRotationAxis(Matrix3& result, float x, float y, float z, float phi);
	static void CreateScaling(Matrix3& result, float x, float y, float z);
	static void Multiply(Matrix3& result, Matrix3& matrix1, Matrix3& matrix2);
	static void Transpose(Matrix3& result, Matrix3& source);
	static void Invert(Matrix3& result, Matrix3& source);
};

#endif
// Matrix3_h
