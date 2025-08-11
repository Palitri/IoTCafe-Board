#ifndef AccelerometerGyroscopeMPU6050_h
#define AccelerometerGyroscopeMPU6050_h

#include "Vectors.h"
#include "RotationBuffer.h"
//#include "Matrix3.h"

class AccelerometerGyroscopeMPU6050
{
private:
	static unsigned char DeviceID_MPU6050;

	static unsigned char RegisterID_MPU6050_Power;
	static unsigned char RegisterID_MPU6050_AccelerometerX;
	static unsigned char RegisterID_MPU6050_AccelerometerY;
	static unsigned char RegisterID_MPU6050_AccelerometerZ;
	static unsigned char RegisterID_MPU6050_AccelerometerTemp;
	static unsigned char RegisterID_MPU6050_GyroscopeX;
	static unsigned char RegisterID_MPU6050_GyroscopeY;
	static unsigned char RegisterID_MPU6050_GyroscopeZ;
	static unsigned char RegisterID_MPU6050_AccelerometerRange;
	static unsigned char RegisterID_MPU6050_GyroscopeRange;
	
	static unsigned char RegisterValue_MPU6050_Power_Clock_Internal8MHz;
	static unsigned char RegisterValue_MPU6050_Power_Clock_PLLGyroX;
	static unsigned char RegisterValue_MPU6050_Power_Clock_PLLGyroY;
	static unsigned char RegisterValue_MPU6050_Power_Clock_PLLGyroZ;
	static unsigned char RegisterValue_MPU6050_Power_Clock_PLLExternal32kHz768;
	static unsigned char RegisterValue_MPU6050_Power_Clock_PLLExternal19MHz2;
	static unsigned char RegisterValue_MPU6050_Power_Clock_Stop;
	static unsigned char RegisterValue_MPU6050_Power_TempDisable;
	static unsigned char RegisterValue_MPU6050_Power_Cycle;
	static unsigned char RegisterValue_MPU6050_Power_Sleep;
	static unsigned char RegisterValue_MPU6050_Power_Reset;

	// Greater range means lesser precision, since there is a fixed number of bits for the output values 
	static const unsigned char RegisterValue_MPU6050_AccelerometerRange_2G = 0x0 << 3;
	static const unsigned char RegisterValue_MPU6050_AccelerometerRange_4G = 0x1 << 3;
	static const unsigned char RegisterValue_MPU6050_AccelerometerRange_8G = 0x2 << 3;
	static const unsigned char RegisterValue_MPU6050_AccelerometerRange_16G = 0x3 << 3;

	// Range units: Degreece per second
	// Greater range means lesser precision, since there is a fixed number of bits for the output values 
	static const unsigned char RegisterValue_MPU6050_GyroscopeRange_250dps = 0x0 << 3;
	static const unsigned char RegisterValue_MPU6050_GyroscopeRange_500dps = 0x1 << 3;
	static const unsigned char RegisterValue_MPU6050_GyroscopeRange_1000dps = 0x2 << 3;
	static const unsigned char RegisterValue_MPU6050_GyroscopeRange_2000dps = 0x3 << 3;

	float accelerometerRotationAnglesFactor;

	float accelerometerResolution;  // G per raw unit
	float accelerometerRange;       // G
	float gyroscopeResolution;      // Turns per second per raw unit
	float gyroscopeRange;           // Turns per second

	int samplingTimeMilliseconds;   // Samples per second

	int awaitingData;

	void* i2c;

	bool RequestData();
	bool ReadData();

	void ComputeFloatVectors();

public:
	//RotationBuffer<float> accelerometerAveragesX, accelerometerAveragesY, accelerometerAveragesZ;
	//RotationBuffer<float> temperatureAverages;

	Vector3I16
		accelerometerRaw, accelerometerRawCorrection,
		gyroscopeRaw, gyroscopeRawCorrection;
	short temperatureRaw;
	Vector2 accelerometerRotationCalibration;

	Vector3 accelerometer;          // Vector pointing towards the orientation of acceleration, which for stationary objects is down. The vector length corresponds to the strength of acceleration, where one unit length means one G
	Vector3 accelerationVector;     // Vector pointing towards the orientation of acceleration, which for stationary objects is down. Normalized to always have length of 1. The scalar acceleration strength is contained in the vectorStrength member
	Vector2 accelerometerRotation;  // Vector holding the angles of rotation accoring to the accelerometer;
	Vector3 gyroscope;              // Vector holding the current momentary rotation along the device's local axes in turns per second

	Vector3 gyroscopeCorrection;    
	float accelerationStrength;     // The strength of acceleration. A value of 1 means one G
	float temperature;              // The temperature in degreece celsius

	Vector3 deltaRotation;    // Vector holding the angles of rotation, based on the momentary rotation and a given time. Unit is turns
	Vector3 rotation;         // Vector holding the angles of rotation on the extrinsic coordinate system. Unit is turns
	//Matrix3 orientation;

	void Init(int i2cIndex);
	void Init(int i2cIndex, int accelerometerRange, int gyroscopeRange);
	void ResetCalibration();
	void Calibrate(int numSamples = 20, int timeoutMilliseconds = 200);
	void SetRegister(unsigned char deviceId, unsigned char registerAddress, unsigned char registerValue);
	void SetRanges(unsigned char accelerometerRange, unsigned char gyroscopeRange);
	bool UpdateReadings();
	//void ReadData(int numSamples);
	//void SetAveragingSamplesCount(int accelerometerSamples, int temperatureSamples);
	void ComputeRotation(float time);
};

#endif
// AccelerometerGyroscopeMPU6050_h

