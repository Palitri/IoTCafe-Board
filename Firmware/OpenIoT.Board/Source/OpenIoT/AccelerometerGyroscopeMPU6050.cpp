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

const unsigned char AccelerometerGyroscopeMPU6050::DeviceID_MPU6050											= 0x68;

const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_Power									= 0x6B;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerX						= 0x3B;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerY						= 0x3D;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerZ						= 0x3F;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerTemp						= 0x41;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeX							= 0x43;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeY							= 0x45;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeZ							= 0x47;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerRange					= 0x1C;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_GyroscopeRange						= 0x1B;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_LowPassFilter							= 0x1A;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_SampleRateDivisor						= 0x19;

const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_Internal8MHz			= 0x0;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLGyroX				= 0x1;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLGyroY				= 0x2;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLGyroZ				= 0x3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLExternal32kHz768	= 0x4;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_PLLExternal19MHz2		= 0x5;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Clock_Stop					= 0x7;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_TempDisable					= 0x1 << 3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_AccelerometerOnly			= 0x1 << 5;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Sleep						= 0x1 << 6;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_Power_Reset						= 0x1 << 7;

const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_AccelerometerRange_2G				= 0x0 << 3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_AccelerometerRange_4G				= 0x1 << 3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_AccelerometerRange_8G				= 0x2 << 3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_AccelerometerRange_16G				= 0x3 << 3;

const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_GyroscopeRange_250dps				= 0x0 << 3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_GyroscopeRange_500dps				= 0x1 << 3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_GyroscopeRange_1000dps				= 0x2 << 3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_GyroscopeRange_2000dps				= 0x3 << 3;

const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_LowPassFilter_260Hz				= 0;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_LowPassFilter_184Hz				= 1;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_LowPassFilter_94Hz					= 2;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_LowPassFilter_44Hz					= 3;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_LowPassFilter_21Hz					= 4;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_LowPassFilter_10Hz					= 5;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_LowPassFilter_5Hz					= 6;
const unsigned char AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_LowPassFilter_Default				= RegisterValue_MPU6050_LowPassFilter_260Hz;

void AccelerometerGyroscopeMPU6050::Init(int i2cIndex)
{
	this->Init(i2cIndex, AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_AccelerometerRange_8G, AccelerometerGyroscopeMPU6050::RegisterValue_MPU6050_GyroscopeRange_500dps);
}

void AccelerometerGyroscopeMPU6050::Init(int i2cIndex, int accelerometerRange, int gyroscopeRange)
{
	this->i2c = Board::I2CBegin(i2cIndex);
	
	//this->SetRegister(AccelerometerGyroscopeMPU6050::DeviceID_MPU6050, AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_Power, RegisterValue_MPU6050_Power_Clock_Internal8MHz);
	this->SetRegister(AccelerometerGyroscopeMPU6050::DeviceID_MPU6050, AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_Power, RegisterValue_MPU6050_Power_Clock_PLLGyroX);
	this->SetRanges(accelerometerRange, gyroscopeRange);

	this->rotation.Set(0.0f);
	this->accelerometerRotationAnglesFactor = 0.02f;
	//Matrix3::CreateIdentity(this->orientation);

	this->samplingTimeMilliseconds = 1000 / 250; // More precisely, it's 1000 / 260 for accelerometer and 1000 / 256 for gyro (defaults), but rounded to a bit longer time 1000/250 for both. See Device registers map specs, registers 25 and 26. Also, consider using interrupt

	this->awaitingData = false;

	this->ResetCalibration();
}

void AccelerometerGyroscopeMPU6050::ResetCalibration()
{
	this->gyroscopeZero.Set(0.0f);
	
	this->accelerometerZero.Set(0.0f, 0.0f, 1.0f);
	this->accelerometerZeroNormalized = this->accelerometerZero;
}


void AccelerometerGyroscopeMPU6050::Calibrate(int numSamples, int timeoutMilliseconds)
{
	this->ResetCalibration();

	unsigned long time = Board::TimeMillis();
	int samples = 0;

	while (samples < numSamples)
	{
		unsigned long newTime = Board::TimeMillis();
		
		if (this->UpdateReadings(false))
		{
			Vector3::Add(this->gyroscopeZero, this->gyroscopeZero, this->gyroscope);
			Vector3::Add(this->accelerometerZero, this->accelerometerZero, this->accelerometer);

			samples++;
		}

		if (newTime - time > timeoutMilliseconds)
			break;
	}

	if (samples == 0)
		return;

	float scaling = 1.0f / (float)samples;
	
	Vector3::Scale(this->gyroscopeZero, this->gyroscopeZero, scaling);
	
	Vector3::Scale(this->accelerometerZero, this->accelerometerZero, scaling);
	Vector3::Normalize(this->accelerometerZeroNormalized, this->accelerometerZero);
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

bool AccelerometerGyroscopeMPU6050::RequestData()
{
	Board::I2CBeginWrite(this->i2c, AccelerometerGyroscopeMPU6050::DeviceID_MPU6050);
	Board::I2CWrite(this->i2c, &AccelerometerGyroscopeMPU6050::RegisterID_MPU6050_AccelerometerX, 1); // Set read address offset to X
	Board::I2CEndWrite(this->i2c);

	Board::I2CRequestData(this->i2c, AccelerometerGyroscopeMPU6050::DeviceID_MPU6050, 14); // Request 14 bytes of data from the read offset onward (Ax, Ay, Az, T, Gx, Gy, Gz)

	return true;
}

bool AccelerometerGyroscopeMPU6050::ReadData()
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

		return true;
	}

	return false;
}

bool AccelerometerGyroscopeMPU6050::UpdateReadings(bool applyCorrections)
{
	if (!this->awaitingData)
	{
		this->RequestData();
		this->awaitingData = true;
	}

	if (this->awaitingData)
	{
		if (this->ReadData())
		{
			this->ComputeFloatVectors();

			if (applyCorrections)
				this->ApplyCorrections();

			this->CalculateAccelerometerRotation();

			this->awaitingData = false;
			return true;
		}
	}

	return false;
}

void AccelerometerGyroscopeMPU6050::ComputeFloatVectors()
{
	this->accelerometer.Set(accelerometerRaw.x, accelerometerRaw.y, accelerometerRaw.z);
	Vector3::Scale(this->accelerometer, this->accelerometer, this->accelerometerResolution);
	Vector3::Normalize(this->accelerometerNormalized, this->accelerometer);
	this->acceleration = Vector3::Length(this->accelerometer);

	this->gyroscope.Set(gyroscopeRaw.x, gyroscopeRaw.y, gyroscopeRaw.z);
	Vector3::Scale(this->gyroscope, this->gyroscope, this->gyroscopeResolution);

	this->temperature = (float)(this->temperatureRaw) / 340.0f + 36.53f;
}

void AccelerometerGyroscopeMPU6050::ApplyCorrections()
{
	Vector3::Subtract(this->gyroscope, this->gyroscope, this->gyroscopeZero);
}

void AccelerometerGyroscopeMPU6050::ComputeRotation(float time)
{
	Vector3 gyroscopeRotation;
	Vector3::Scale(gyroscopeRotation, this->gyroscope, time);
	Vector3::Add(this->rotation, this->rotation, gyroscopeRotation);

	// const float alphaBlendAccelerometerPerSecond = 1.0f;
	// float blendFactor = 0.00001f;//time * alphaBlendAccelerometerPerSecond;
	// this->rotation.x += (this->accelerometerRotation.x - this->rotation.x) * blendFactor;
	// this->rotation.y += (this->accelerometerRotation.y - this->rotation.y) * blendFactor;
}

void AccelerometerGyroscopeMPU6050::CalculateAccelerometerRotation()
{
	float dot = Vector3::Dot(this->accelerometerNormalized, this->accelerometerZeroNormalized);
	float rotation = Math::ArcCos(dot) / Math::Pi2;
	this->accelerometerRotation.x = this->accelerometerNormalized.y * rotation;
	this->accelerometerRotation.y = -this->accelerometerNormalized.x * rotation;
}
