#ifndef AccelerometerGyroscopeMPU6050_h
#define AccelerometerGyroscopeMPU6050_h

#include "Vectors.h"
#include "RotationBuffer.h"
//#include "Matrix3.h"

class AccelerometerGyroscopeMPU6050
{
private:
	static const unsigned char DeviceID_MPU6050;

	static const unsigned char RegisterID_MPU6050_Power;								// Power mode and clock setting. Predefined values.
	static const unsigned char RegisterID_MPU6050_AccelerometerX;						// Accelerometer X reading output
	static const unsigned char RegisterID_MPU6050_AccelerometerY;						// Accelerometer Y reading output
	static const unsigned char RegisterID_MPU6050_AccelerometerZ;						// Accelerometer Z reading output
	static const unsigned char RegisterID_MPU6050_AccelerometerTemp;					// Temperature reading output
	static const unsigned char RegisterID_MPU6050_GyroscopeX;							// Gyroscope X reading output
	static const unsigned char RegisterID_MPU6050_GyroscopeY;							// Gyroscope Y reading output
	static const unsigned char RegisterID_MPU6050_GyroscopeZ;							// Gyroscope Z reading output
	static const unsigned char RegisterID_MPU6050_AccelerometerRange;					// Maximum acceleration. Predefined values. Greater range means lesser precision, since there is a fixed number of bits for the output values 
	static const unsigned char RegisterID_MPU6050_GyroscopeRange;						// Maximum rotation in degreece per second. Predefined values.
	static const unsigned char RegisterID_MPU6050_LowPassFilter;						// Low pass filter for filtering out noise. Predefined values.
	static const unsigned char RegisterID_MPU6050_SampleRateDivisor;					// Allows to take samples in lower rate such that resulting sampling rate =  SamplingRate / (1 + value). Default is 0. Non-predefined values - any positive int can be set

	static const unsigned char RegisterValue_MPU6050_Power_Clock_Internal8MHz;			// Uses internal 8Mhz oscillator
	static const unsigned char RegisterValue_MPU6050_Power_Clock_PLLGyroX;				// Phase-locked loop using the gyroscope's own X axis oscillator
	static const unsigned char RegisterValue_MPU6050_Power_Clock_PLLGyroY;				// Phase-locked loop using the gyroscope's own Y axis oscillator
	static const unsigned char RegisterValue_MPU6050_Power_Clock_PLLGyroZ;				// Phase-locked loop using the gyroscope's own Z axis oscillator
	static const unsigned char RegisterValue_MPU6050_Power_Clock_PLLExternal32kHz768;	// Phase-locked loop with external 32.768kHz oscillator
	static const unsigned char RegisterValue_MPU6050_Power_Clock_PLLExternal19MHz2;		// Phase-locked loop with external 19.2MHz oscillator
	static const unsigned char RegisterValue_MPU6050_Power_Clock_Stop;					// Stop
	static const unsigned char RegisterValue_MPU6050_Power_TempDisable;					// Disables the temperature sensor
	static const unsigned char RegisterValue_MPU6050_Power_AccelerometerOnly;			// Turns off all other features but the accelerometer for low-power mode
	static const unsigned char RegisterValue_MPU6050_Power_Sleep;						// Sets the device to sleep mode
	static const unsigned char RegisterValue_MPU6050_Power_Reset;						// Resets the device to hardware defaults

	static const unsigned char RegisterValue_MPU6050_AccelerometerRange_2G;				// Up to 2G acceleration
	static const unsigned char RegisterValue_MPU6050_AccelerometerRange_4G;				// Up to 4G acceleration
	static const unsigned char RegisterValue_MPU6050_AccelerometerRange_8G;				// Up to 8G acceleration
	static const unsigned char RegisterValue_MPU6050_AccelerometerRange_16G;			// Up to 16G acceleration

	static const unsigned char RegisterValue_MPU6050_GyroscopeRange_250dps;				// Up to 250 degreece rotation per second
	static const unsigned char RegisterValue_MPU6050_GyroscopeRange_500dps;				// Up to 500 degreece rotation per second
	static const unsigned char RegisterValue_MPU6050_GyroscopeRange_1000dps;			// Up to 1000 degreece rotation per second
	static const unsigned char RegisterValue_MPU6050_GyroscopeRange_2000dps;			// Up to 2000 degreece rotation per second
	
	static const unsigned char RegisterValue_MPU6050_LowPassFilter_260Hz;				// Bandwidth: 260Hz		Delay: 0ms		Sampling rate: 8kHz / (1 + SampleRateDivisor)
	static const unsigned char RegisterValue_MPU6050_LowPassFilter_184Hz;				// Bandwidth: 184Hz		Delay: 2ms		Sampling rate: 1kHz / (1 + SampleRateDivisor)
	static const unsigned char RegisterValue_MPU6050_LowPassFilter_94Hz;				// Bandwidth: 94Hz		Delay: 3ms		Sampling rate: 1kHz / (1 + SampleRateDivisor)
	static const unsigned char RegisterValue_MPU6050_LowPassFilter_44Hz;				// Bandwidth: 44Hz		Delay: 4.9ms	Sampling rate: 1kHz / (1 + SampleRateDivisor)
	static const unsigned char RegisterValue_MPU6050_LowPassFilter_21Hz;				// Bandwidth: 21Hz		Delay: 8.5ms	Sampling rate: 1kHz / (1 + SampleRateDivisor)
	static const unsigned char RegisterValue_MPU6050_LowPassFilter_10Hz;				// Bandwidth: 10Hz		Delay: 13.8ms	Sampling rate: 1kHz / (1 + SampleRateDivisor)
	static const unsigned char RegisterValue_MPU6050_LowPassFilter_5Hz;					// Bandwidth: 5Hz		Delay: 19ms		Sampling rate: 1kHz / (1 + SampleRateDivisor)
	static const unsigned char RegisterValue_MPU6050_LowPassFilter_Default;

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
	void ApplyCorrections();

	void CalculateAccelerometerRotation();

public:
	Vector3I16 accelerometerRaw, gyroscopeRaw;
	short temperatureRaw;

	Vector3 accelerometer;				// Vector pointing towards the orientation of acceleration, which for stationary objects is down. The vector length corresponds to the strength of acceleration, where one unit length means one G
	Vector3 accelerometerNormalized;	// Vector pointing towards the orientation of acceleration, which for stationary objects is down. Normalized to always have length of 1. The scalar acceleration strength is contained in the vectorStrength member
	Vector2 accelerometerRotation;		// Vector holding the angles of rotation accoring to the accelerometer;
	Vector3 accelerometerZero, accelerometerZeroNormalized;
	float acceleration;					// The magnitude of acceleration, In G

	Vector3 gyroscope;					// Vector holding the current momentary rotation along the device's local axes in turns per second
	Vector3 gyroscopeZero;				// Value at calibration state
	float temperature;					// The temperature in degreece celsius

	Vector3 deltaRotation;    // Vector holding the angles of rotation, based on the momentary rotation and a given time. Unit is turns
	Vector3 rotation;         // Vector holding the angles of rotation on the extrinsic coordinate system. Unit is turns
	//Matrix3 orientation;

	void Init(int i2cIndex);
	void Init(int i2cIndex, int accelerometerRange, int gyroscopeRange);
	void ResetCalibration();
	void Calibrate(int numSamples = 20, int timeoutMilliseconds = 200);
	void SetRegister(unsigned char deviceId, unsigned char registerAddress, unsigned char registerValue);
	void SetRanges(unsigned char accelerometerRange, unsigned char gyroscopeRange);
	bool UpdateReadings(bool applyCorrections = true);
	//void ReadData(int numSamples);
	//void SetAveragingSamplesCount(int accelerometerSamples, int temperatureSamples);
	void ComputeRotation(float time);
};

#endif
// AccelerometerGyroscopeMPU6050_h

