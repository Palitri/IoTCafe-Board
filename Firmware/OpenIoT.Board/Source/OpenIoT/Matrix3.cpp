//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "Matrix3.h"

#include "Math.h"

float Matrix3::GetDeterminant(Matrix3 &matrix)
{
  return matrix.m12 * matrix.m23 * matrix.m31 +
    matrix.m13 * matrix.m21 * matrix.m32 -
    matrix.m11 * matrix.m23 * matrix.m32 -
    matrix.m12 * matrix.m21 * matrix.m33 -
    matrix.m13 * matrix.m22 * matrix.m31;
}

void Matrix3::CreateIdentity(Matrix3 &result)
{
  result.m11 = 1.0f;
  result.m12 = 0.0f;
  result.m13 = 0.0f;
  result.m21 = 0.0f;
  result.m22 = 1.0f;
  result.m23 = 0.0f;
  result.m31 = 0.0f;
  result.m32 = 0.0f;
  result.m33 = 1.0f;
}
  
void Matrix3::CreateRotationX(Matrix3 &result, float phi)
{
  float SinPhi = Math::Sin(phi);
  float CosPhi = Math::Cos(phi);

  result.m11 = 1.0f;
  result.m12 = 0.0f;
  result.m13 = 0.0f;
  result.m21 = 0.0f;
  result.m22 = CosPhi;
  result.m23 = SinPhi;
  result.m31 = 0.0f;
  result.m32 = -SinPhi;
  result.m33 = CosPhi;
}

void Matrix3::CreateRotationY(Matrix3 &result, float phi)
{
  float SinPhi = Math::Sin(phi);
  float CosPhi = Math::Cos(phi);

  result.m11 = CosPhi;
  result.m12 = 0.0f;
  result.m13 = -SinPhi;
  result.m21 = 0.0f;
  result.m22 = 1.0f;
  result.m23 = 0.0f;
  result.m31 = SinPhi;
  result.m32 = 0.0f;
  result.m33 = CosPhi;
}

void Matrix3::CreateRotationZ(Matrix3 &result, float phi)
{
  float SinPhi = Math::Sin(phi);
  float CosPhi = Math::Cos(phi);

  result.m11 = CosPhi;
  result.m12 = SinPhi;
  result.m13 = 0.0f;
  result.m21 = -SinPhi;
  result.m22 = CosPhi;
  result.m23 = 0.0f;
  result.m31 = 0.0f;
  result.m32 = 0.0f;
  result.m33 = 1.0f;
} 

void Matrix3::CreateRotationAxis(Matrix3 &result, float x, float y, float z, float phi)
{
  float SinPhi = Math::Sin(phi);
  float CosPhi = Math::Cos(phi);
  float InvCosPhi = 1 - CosPhi;
  float SinPhiX = SinPhi * x;
  float SinPhiY = SinPhi * y;
  float SinPhiZ = SinPhi * z;
  float InvCosPhiXY = InvCosPhi * x * y;
  float InvCosPhiXZ = InvCosPhi * x * z;
  float InvCosPhiYZ = InvCosPhi * y * z;

  result.m11 = CosPhi + InvCosPhi * x * x;
  result.m12 = InvCosPhiXY - SinPhiZ;
  result.m13 = InvCosPhiXZ + SinPhiY;
  result.m21 = InvCosPhiXY + SinPhiZ;
  result.m22 = CosPhi + InvCosPhi * y * y;
  result.m23 = InvCosPhiYZ - SinPhiX;
  result.m31 = InvCosPhiXZ - SinPhiY;
  result.m32 = InvCosPhiYZ + SinPhiX;
  result.m33 = CosPhi + InvCosPhi * z * z;
}

void Matrix3::CreateScaling(Matrix3 &result, float x, float y, float z)
{
  result.m11 = x;
  result.m12 = 0.0f;
  result.m13 = 0.0f;
  result.m21 = 0.0f;
  result.m22 = y;
  result.m23 = 0.0f;
  result.m31 = 0.0f;
  result.m32 = 0.0f;
  result.m33 = z;
}

void Matrix3::Multiply(Matrix3 &result, Matrix3 &matrix1, Matrix3 &matrix2)
{
  float m11 =   matrix1.m11 * matrix2.m11 +
                matrix1.m12 * matrix2.m21 +
                matrix1.m13 * matrix2.m31;
  float m12 =   matrix1.m11 * matrix2.m12 +
                matrix1.m12 * matrix2.m22 +
                matrix1.m13 * matrix2.m32;
  float m13 =   matrix1.m11 * matrix2.m13 +
                matrix1.m12 * matrix2.m23 +
                matrix1.m13 * matrix2.m33;
  float m21 =   matrix1.m21 * matrix2.m11 +
                matrix1.m22 * matrix2.m21 +
                matrix1.m23 * matrix2.m31;
  float m22 =   matrix1.m21 * matrix2.m12 +
                matrix1.m22 * matrix2.m22 +
                matrix1.m23 * matrix2.m32;
  float m23 =   matrix1.m21 * matrix2.m13 +
                matrix1.m22 * matrix2.m23 +
                matrix1.m23 * matrix2.m33;
  float m31 =   matrix1.m31 * matrix2.m11 +
                matrix1.m32 * matrix2.m21 +
                matrix1.m33 * matrix2.m31;
  float m32 =   matrix1.m31 * matrix2.m12 +
                matrix1.m32 * matrix2.m22 +
                matrix1.m33 * matrix2.m32;
  float m33 =   matrix1.m31 * matrix2.m13 +
                matrix1.m32 * matrix2.m23 +
                matrix1.m33 * matrix2.m33;

  result.m11 = m11;
  result.m12 = m12;
  result.m13 = m13;
  result.m21 = m21;
  result.m22 = m22;
  result.m23 = m23;
  result.m31 = m31;
  result.m32 = m32;
  result.m33 = m33;
}

void Matrix3::Transpose(Matrix3 &result, Matrix3 &source)
{
    float m11 = source.m11;
    float m12 = source.m21;
    float m13 = source.m31;
    float m21 = source.m12;
    float m22 = source.m22;
    float m23 = source.m32;
    float m31 = source.m13;
    float m32 = source.m23;
    float m33 = source.m33;
        
    result.m11 = m11;
    result.m12 = m12;
    result.m13 = m13;
    result.m21 = m21;
    result.m22 = m22;
    result.m23 = m23;
    result.m31 = m31;
    result.m32 = m32;
    result.m33 = m33;
}

void Matrix3::Invert(Matrix3 &result, Matrix3 &source)
{
  float det = Matrix3::GetDeterminant(source);

  if (det == 0.0f)
    return;

  det = 1.0f / det;

  float m11 = det * (source.m22 * source.m33 - source.m23 * source.m32);
  float m12 = det * (source.m13 * source.m32 - source.m12 * source.m33);
  float m13 = det * (source.m12 * source.m23 - source.m13 * source.m22);
  float m21 = det * (source.m23 * source.m31 - source.m21 * source.m33);
  float m22 = det * (source.m11 * source.m33 - source.m13 * source.m31);
  float m23 = det * (source.m13 * source.m21 - source.m11 * source.m23);
  float m31 = det * (source.m21 * source.m32 - source.m22 * source.m31);
  float m32 = det * (source.m12 * source.m31 - source.m11 * source.m32);
  float m33 = det * (source.m11 * source.m22 - source.m12 * source.m21);

  result.m11 = m11;
  result.m12 = m12;
  result.m13 = m13;
  result.m21 = m21;
  result.m22 = m22;
  result.m23 = m23;
  result.m31 = m31;
  result.m32 = m32;
  result.m33 = m33;
}

