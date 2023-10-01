//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "AccelerometerGyroscopeMPU6050.h"

#include "Board.h"
#include "Math.h"

unsigned char AccelerometerGyroscopeMPU6050::DeviceID_MPU6050 = 0x68;

unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_Power = 0x6B;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerX = 0x3B;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerY = 0x3D;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerZ = 0x3F;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerTemp = 0x41;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeX = 0x43;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeY = 0x45;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeZ = 0x47;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerRange = 0x1C;
unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeRange = 0x1B;

unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_Internal8MHz = 0x0;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLGyroX = 0x1;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLGyroY = 0x2;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLGyroZ = 0x3;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLExternal32kHz768 = 0x4;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLExternal19MHz2 = 0x5;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_Stop = 0x7;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_TempDisable = 0x1 << 3;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Cycle = 0x1 << 5;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Sleep = 0x1 << 6;
unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Reset = 0x1 << 7;

void AccelerometerGyroscopeMPU6050::Init(int i2cIndex)
{
	this->Init(i2cIndex, AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_AccelerometerRange_8G, AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_GyroscopeRange_500dps);
}

void AccelerometerGyroscopeMPU6050::Init(int i2cIndex, int accelerometerRange, int gyroscopeRange)
{
	this->i2c = Board::I2CBegin(i2cIndex);
	
	this->SetRegister(AccelerometerGyroscopeMPU6050::DeviceID_MPU6050, AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_Power, RegisterValue_MPU6050_Power_Clock_Internal8MHz);
	this->SetRanges(accelerometerRange, gyroscopeRange);

	this->accelerometerRawCorrection.Set(0.0f);
	this->gyroscopeRawCorrection.Set(0.0f);

	this->rotation.Set(0.0f);
	this->accelerometerRotationAnglesFactor = 0.02f;
	//Matrix3::CreateIdentity(this->orientation);

	this->samplingTimeMilliseconds = 1000 / 250; // More precisely, it's 1000 / 260 for accelerometer and 1000 / 256 for gyro (defaults), but rounded to a bit longer time 1000/250 for both. See Device registers map specs, registers 25 and 26. Also, consider using interrupt

	this->awaitingData = false;
}

void AccelerometerGyroscopeMPU6050::Calibrate(int numSamples)
{
	// Zero corrections, so that they don't affect measurement
	this->accelerometerRawCorrection.Set(0.0f);
	this->gyroscopeRawCorrection.Set(0.0f);

	//this->ReadData(numSamples);
	this->ReadData();
	this->ComputeFloatVectors();

	this->accelerometerRotationCalibration.Set(0.0f);
	this->ComputeRotation(0.0f);
	this->accelerometerRotationCalibration = this->accelerometerRotation;


	// Calibrating accelerometer will require more complex computations than just linear correction
	//Vector3I accelerometerRawAtRest;
	//accelerometerRawAtRest.Set(0, 0, -(int)(1.0f / this->accelerometerResolution));
	//Vector3I::Subtract(this->accelerometerRawCorrection, accelerometerRawAtRest, this->accelerometerRaw);

	Vector3I16::Invert(this->gyroscopeRawCorrection, this->gyroscopeRaw);
}

void AccelerometerGyroscopeMPU6050::SetRanges(unsigned char accelerometerRange, unsigned char gyroscopeRange)
{
	this->SetRegister(AccelerometerGyroscopeMPU6050::DeviceID_MPU6050, AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerRange, accelerometerRange);
	this->SetRegister(AccelerometerGyroscopeMPU6050::DeviceID_MPU6050, AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeRange, gyroscopeRange);

	switch (accelerometerRange)
	{
		case RegisterValue_MPU6050_AccelerometerRange_2G:
		{
			this->accelerometerRange = 2.0f;
			break;
		}
		case RegisterValue_MPU6050_AccelerometerRange_4G:
		{
			this->accelerometerRange = 4.0f;
			break;
		}
		case RegisterValue_MPU6050_AccelerometerRange_8G:
		{
			this->accelerometerRange = 8.0f;
			break;
		}
		case RegisterValue_MPU6050_AccelerometerRange_16G:
		{
			this->accelerometerRange = 16.0f;
			break;
		}
	}
	this->accelerometerResolution = this->accelerometerRange / (float)0x8000;

	switch (gyroscopeRange)
	{
		case RegisterValue_MPU6050_GyroscopeRange_250dps:
		{
			this->gyroscopeRange = 250.0f;
			break;
		}
		case RegisterValue_MPU6050_GyroscopeRange_500dps:
		{
			this->gyroscopeRange = 500.0f;
			break;
		}
		case RegisterValue_MPU6050_GyroscopeRange_1000dps:
		{
			this->gyroscopeRange = 1000.0f;
			break;
		}
		case RegisterValue_MPU6050_GyroscopeRange_2000dps:
		{
			this->gyroscopeRange = 2000.0f;
			break;
		}
	}

	this->gyroscopeRange = this->gyroscopeRange / 360.0f;
	this->gyroscopeResolution = this->gyroscopeRange / (float)0x8000;
}

void AccelerometerGyroscopeMPU6050::SetRegister(unsigned char deviceId, unsigned char registerAddress, unsigned char registerValue)
{
	Board::I2CBeginWrite(this->i2c, deviceId);
	Board::I2CWrite(this->i2c , &registerAddress, 1);
	Board::I2CWrite(this->i2c , &registerValue, 1);
	Board::I2CEndWrite(this->i2c);
}

void AccelerometerGyroscopeMPU6050::ReadData()
{
	if (!this->awaitingData)
	{
		Board::I2CBeginWrite(this->i2c, AccelerometerGyroscopeMPU6050::DeviceID_MPU6050);
		Board::I2CWrite(this->i2c, &AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerX, 1); // Set read address offset to X
		Board::I2CEndWrite(this->i2c);

		Board::I2CRequestData(this->i2c, AccelerometerGyroscopeMPU6050::DeviceID_MPU6050, 14); // Request 14 bytes of data from the read offset onward (Ax, Ay, Az, T, Gx, Gy, Gz)
		this->awaitingData = true;
	}

	if (this->awaitingData)
	{
		if (Board::I2CBytesAvailable(this->i2c) >= 14)
		{
			Board::I2CReadMsbFirst(this->i2c, &this->accelerometerRaw.x, 2);
			Board::I2CReadMsbFirst(this->i2c, &this->accelerometerRaw.y, 2);
			Board::I2CReadMsbFirst(this->i2c, &this->accelerometerRaw.z, 2);
			Board::I2CReadMsbFirst(this->i2c, &this->temperatureRaw, 2);
			Board::I2CReadMsbFirst(this->i2c, &this->gyroscopeRaw.x, 2);
			Board::I2CReadMsbFirst(this->i2c, &this->gyroscopeRaw.y, 2);
			Board::I2CReadMsbFirst(this->i2c, &this->gyroscopeRaw.z, 2);

			this->awaitingData = false;
		}
	}

	//Vector3I::Add(this->accelerometerRaw, this->accelerometerRaw, this->accelerometerRawCorrection);
	//Vector3I16::Add(this->gyroscopeRaw, this->gyroscopeRaw, this->gyroscopeRawCorrection);
}

//void AccelerometerGyroscopeMPU6050::ReadData(int numSamples)
//{
//	long accelerometerSum_x = 0;
//	long accelerometerSum_y = 0;
//	long accelerometerSum_z = 0;
//	long temperatureSum = 0;
//	long gyroscopeSum_x = 0;
//	long gyroscopeSum_y = 0;
//	long gyroscopeSum_z = 0;
//
//	for (int i = numSamples; i > 0; i--)
//	{
//		this->ReadData();
//
//		accelerometerSum_x += this->accelerometerRaw.x;
//		accelerometerSum_y += this->accelerometerRaw.y;
//		accelerometerSum_z += this->accelerometerRaw.z;
//		temperatureSum += this->temperatureRaw;
//		gyroscopeSum_x += this->gyroscopeRaw.x;
//		gyroscopeSum_y += this->gyroscopeRaw.y;
//		gyroscopeSum_z += this->gyroscopeRaw.z;
//
//		Board::DelayMillis(this->samplingTimeMilliseconds);
//	}
//
//	this->accelerometerRaw.x = accelerometerSum_x / numSamples;
//	this->accelerometerRaw.y = accelerometerSum_y / numSamples;
//	this->accelerometerRaw.z = accelerometerSum_z / numSamples;
//	this->temperatureRaw = temperatureSum / numSamples;
//	this->gyroscopeRaw.x = gyroscopeSum_x / numSamples;
//	this->gyroscopeRaw.y = gyroscopeSum_y / numSamples;
//	this->gyroscopeRaw.z = gyroscopeSum_z / numSamples;
//}


//void AccelerometerGyroscopeMPU6050::SetAveragingSamplesCount(int accelerometerSamples, int temperatureSamples)
//{
//	this->accelerometerAveragesX.SetSize(accelerometerSamples);
//	this->accelerometerAveragesY.SetSize(accelerometerSamples);
//	this->accelerometerAveragesZ.SetSize(accelerometerSamples);
//	this->temperatureAverages.SetSize(temperatureSamples);
//}

void AccelerometerGyroscopeMPU6050::ComputeFloatVectors()
{
	this->accelerometer.Set(accelerometerRaw.y, -accelerometerRaw.x, -accelerometerRaw.z);
	Vector3::Scale(this->accelerometer, this->accelerometer, this->accelerometerResolution);

	//if (this->accelerometerAveragesX.size != 0)
	//{
	//	this->accelerometerAveragesX.Add(this->accelerometer.x);
	//	this->accelerometerAveragesY.Add(this->accelerometer.y);
	//	this->accelerometerAveragesZ.Add(this->accelerometer.z);
	//	this->accelerometer.Set(this->accelerometerAveragesX.mean, this->accelerometerAveragesY.mean, this->accelerometerAveragesZ.mean);
	//}

	// Don't normalize for now, because accelerometer Z value gives strange values and because of it, normalization distorts X and Y as well
	//this->accelerationStrength = Vector3::Normalize(this->accelerationVector, this->accelerometer);
	this->accelerationVector = this->accelerometer;




	this->gyroscope.Set(gyroscopeRaw.y, gyroscopeRaw.x, gyroscopeRaw.z);
	Vector3::Scale(this->gyroscope, this->gyroscope, this->gyroscopeResolution);



	this->temperature = (float)(this->temperatureRaw) / 340.0f + 36.53f;
	//if (this->temperatureAverages.size != 0)
	//{
	//	this->temperatureAverages.Add(this->temperature);
	//	this->temperature = this->temperatureAverages.mean;
	//}
}

void AccelerometerGyroscopeMPU6050::ComputeRotation(float time)
{
	this->accelerometerRotation.x = Math::ArcSin(Math::Trim(this->accelerationVector.y, -1.0f, 1.0f)) / Math::Pi2 - this->accelerometerRotationCalibration.x;
	this->accelerometerRotation.y = Math::ArcSin(Math::Trim(this->accelerationVector.x, -1.0f, 1.0f)) / Math::Pi2 - this->accelerometerRotationCalibration.y;

	this->deltaRotation.x = this->gyroscope.x * time;
	this->deltaRotation.y = this->gyroscope.y * time;
	this->deltaRotation.z = this->gyroscope.z * time;

	this->rotation.x += this->deltaRotation.x;
	this->rotation.y += this->deltaRotation.y;
	this->rotation.z += this->deltaRotation.z;

	this->rotation.x -= this->rotation.y * Math::Sin(this->deltaRotation.z * Math::Pi2);
	this->rotation.y += this->rotation.x * Math::Sin(this->deltaRotation.z * Math::Pi2);

	this->rotation.x = this->rotation.x + (this->accelerometerRotation.x - this->rotation.x) * this->accelerometerRotationAnglesFactor;
	this->rotation.y = this->rotation.y + (this->accelerometerRotation.y - this->rotation.y) * this->accelerometerRotationAnglesFactor;

	//  Matrix m;
	//  Matrix::CreateRotationX(m, this->deltaRotation.x * Math::Pi2);
	//  Matrix::Multiply(this->orientation, m, this->orientation);
	//  Matrix::CreateRotationY(m, this->deltaRotation.y * Math::Pi2);
	//  Matrix::Multiply(this->orientation, m, this->orientation);
	//  Matrix::CreateRotationZ(m, this->deltaRotation.z * Math::Pi2);
	//  Matrix::Multiply(this->orientation, m, this->orientation);

	//Matrix3 m;
	//Matrix3::CreateRotationX(this->orientation, this->rotation.x * Math::Pi2);
	//Matrix3::CreateRotationY(m, -this->rotation.y * Math::Pi2);
	//Matrix3::Multiply(this->orientation, this->orientation, m);


	//this->intrinsicRotation.x += this->intrinsicRotation.y * Math::Sin(this->gyroscope.z * time * Math::Pi2);
	//this->intrinsicRotation.y -= this->intrinsicRotation.x * Math::Sin(this->gyroscope.z * time * Math::Pi2);

  //  float xCorrection = this->intrinsicRotation.y * Math::Sin(this->gyroscope.z * time * Math::Pi2);
  //  float yCorrection = this->intrinsicRotation.x * Math::Sin(this->gyroscope.z * time * Math::Pi2);
  //  this->intrinsicRotation.x += xCorrection;
  //  this->intrinsicRotation.y -= yCorrection;
}

