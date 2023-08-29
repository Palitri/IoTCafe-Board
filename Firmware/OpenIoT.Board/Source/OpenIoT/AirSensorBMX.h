#ifndef AirSensorBMX_h
#define AirSensorBMX_h

#include "Board.h"

// Bosch Temperature, Pressure, Humidity, *Gas sensors BMP280, BME280, *BME680, *BME688. *unsupported by current version
// Consider renaming to AirSensorBMX280, because the BME680 and 688 are too different - more compensation registers, different addresses
class AirSensorBMX
{
private:
	static const unsigned char DeviceAddress_BMX;

	static const unsigned char RegisterAddress_BMX_ChipId;
	static const unsigned char RegisterAddress_BMX_Reset;
	static const unsigned char RegisterAddress_BMX_Status;
	static const unsigned char RegisterAddress_BMX_SettingsBits;
	static const unsigned char RegisterAddress_BMX_ConfigBits;
	static const unsigned char RegisterAddress_BMX_Pressure;
	static const unsigned char RegisterAddress_BMX_Temperature;
	static const unsigned char RegisterAddress_BMX_Humidity;

	static const unsigned char RegisterAddress_BMX_Compensation_T1;
	static const unsigned char RegisterAddress_BMX_Compensation_T2;
	static const unsigned char RegisterAddress_BMX_Compensation_T3;

	static const unsigned char RegisterAddress_BMX_Compensation_P1;
	static const unsigned char RegisterAddress_BMX_Compensation_P2;
	static const unsigned char RegisterAddress_BMX_Compensation_P3;
	static const unsigned char RegisterAddress_BMX_Compensation_P4;
	static const unsigned char RegisterAddress_BMX_Compensation_P5;
	static const unsigned char RegisterAddress_BMX_Compensation_P6;
	static const unsigned char RegisterAddress_BMX_Compensation_P7;
	static const unsigned char RegisterAddress_BMX_Compensation_P8;
	static const unsigned char RegisterAddress_BMX_Compensation_P9;

	static const unsigned char RegisterAddress_BMX_Compensation_H1;
	static const unsigned char RegisterAddress_BMX_Compensation_H2;
	static const unsigned char RegisterAddress_BMX_Compensation_H3;
	static const unsigned char RegisterAddress_BMX_Compensation_H4;
	static const unsigned char RegisterAddress_BMX_Compensation_H5;
	static const unsigned char RegisterAddress_BMX_Compensation_H6;

	static const unsigned char RegisterValue_BMX_Status_Free;
	static const unsigned char RegisterValue_BMX_StatusBits_Updating;
	static const unsigned char RegisterValue_BMX_StatusBits_Measuring;

	static const unsigned char RegisterValue_BMX_SettingsBits_Power_Sleep;
	static const unsigned char RegisterValue_BMX_SettingsBits_Power_Forced;
	static const unsigned char RegisterValue_BMX_SettingsBits_Power_Normal;

	static const unsigned char RegisterValue_BMX_SettingsBits_Pressure_Disabled;
	static const unsigned char RegisterValue_BMX_SettingsBits_Pressure_16bit; // 2.62 Pa precision
	static const unsigned char RegisterValue_BMX_SettingsBits_Pressure_17bit; // 1.31 Pa precision
	static const unsigned char RegisterValue_BMX_SettingsBits_Pressure_18bit; // 0.66 Pa precision
	static const unsigned char RegisterValue_BMX_SettingsBits_Pressure_19bit; // 0.33 Pa precision
	static const unsigned char RegisterValue_BMX_SettingsBits_Pressure_20bit; // 0.16 Pa precision

	static const unsigned char RegisterValue_BMX_SettingsBits_Temperature_Disabled;
	static const unsigned char RegisterValue_BMX_SettingsBits_Temperature_16bit; // 0.0050 °C precision
	static const unsigned char RegisterValue_BMX_SettingsBits_Temperature_17bit; // 0.0025 °C precision
	static const unsigned char RegisterValue_BMX_SettingsBits_Temperature_18bit; // 0.0012 °C precision
	static const unsigned char RegisterValue_BMX_SettingsBits_Temperature_19bit; // 0.0006 °C precision
	static const unsigned char RegisterValue_BMX_SettingsBits_Temperature_20bit; // 0.0003 °C precision

	static const unsigned char RegisterValue_BMX_Reset;

	static const unsigned char RegisterValue_BMX_ConfigBits_SPI_Disabled;
	static const unsigned char RegisterValue_BMX_ConfigBits_SPI_Enabled;

	static const unsigned char RegisterValue_BMX_ConfigBits_Filter_Disabled;
	static const unsigned char RegisterValue_BMX_ConfigBits_Filter_X2;
	static const unsigned char RegisterValue_BMX_ConfigBits_Filter_X4;
	static const unsigned char RegisterValue_BMX_ConfigBits_Filter_X8;
	static const unsigned char RegisterValue_BMX_ConfigBits_Filter_X16;

	static const unsigned char RegisterValue_BMX_ConfigBits_StandbyTime_0ms5;
	static const unsigned char RegisterValue_BMX_ConfigBits_StandbyTime_62ms5;
	static const unsigned char RegisterValue_BMX_ConfigBits_StandbyTime_125ms;
	static const unsigned char RegisterValue_BMX_ConfigBits_StandbyTime_250ms;
	static const unsigned char RegisterValue_BMX_ConfigBits_StandbyTime_500ms;
	static const unsigned char RegisterValue_BMX_ConfigBits_StandbyTime_1000ms;
	static const unsigned char RegisterValue_BMX_ConfigBits_StandbyTime_2000ms;
	static const unsigned char RegisterValue_BMX_ConfigBits_StandbyTime_4000ms;

	static const unsigned char RegisterValue_BMX_ChipId_BMP280_Sample1;
	static const unsigned char RegisterValue_BMX_ChipId_BMP280_Sample2;
	static const unsigned char RegisterValue_BMX_ChipId_BMP280;
	static const unsigned char RegisterValue_BMX_ChipId_BME280;
	static const unsigned char RegisterValue_BMX_ChipId_BME680;
	static const unsigned char RegisterValue_BMX_ChipId_BME688;

	static const char* ChipName_BMP280_Sample;
	static const char* ChipName_BMP280;
	static const char* ChipName_BME280;
	static const char* ChipName_BME680;
	static const char* ChipName_BME688;

	int awaitingData;

	// Constant read only parameters, used for "compensation", as referred to in Bosch BME280 specs doc
	// Aligning parameters' names as found in the original Bosch doc for easier reference with their implementation
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;

	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;

	uint8_t dig_H1;
	int16_t dig_H2;
	uint8_t dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;
	int8_t dig_H6;

	int readingBlockSize;
	
	void ReadFeatures();
	void ReadCompensationParameters();

	int32_t GetFineTemperatureFactor(int32_t adc_T);

	float GetTemperatureCompensated(int32_t t_fine);
	float GetPressureCompensated(int32_t adc_P, int32_t t_fine);
	float GetHumidityCompensated(int32_t adc_H, int32_t t_fine);

public:
	void* i2c;

	unsigned char chipId;
	char* chipName;
	bool sensesTemperature;
	bool sensesPressure;
	bool sensesHumidity;
	bool sensesGas;

	float temperature;           // The temperature in degreece celsius
	float pressure;              // The pressure in pascals
	float humidity;             // The humidity in %RH


	void Init(int i2cIndex);
	void Reset();
	unsigned char GetChipId();
	int GetStatus();
	void SetSettingsMode(unsigned char powerMode, unsigned char pressureMode, unsigned char temperatureMode);
	void SetConfigurationMode(unsigned char spiMode, unsigned char filterMode, unsigned char standbyTime);
	void ReadData();

	void SetRegister(unsigned char deviceId, unsigned char registerAddress, unsigned char registerValue);
	void GetRegister(unsigned char deviceId, unsigned char registerAddress, void* dest, int size);
};

#endif
// AirSensorBMX_h
