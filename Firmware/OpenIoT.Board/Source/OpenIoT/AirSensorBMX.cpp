//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "AirSensorBMX.h"

const unsigned char AirSensorBMX::DeviceAddress_BMX = 0x76; // 0x76: SDO pin connected to GND, 0x77: SDO pin connected to Vccio

const unsigned char AirSensorBMX::RegisterAddress_BMX_ChipId = 0xD0;            // 1 byte
const unsigned char AirSensorBMX::RegisterAddress_BMX_Reset = 0xE0;             // 1 byte
const unsigned char AirSensorBMX::RegisterAddress_BMX_Status = 0xF3;            // 1 byte
const unsigned char AirSensorBMX::RegisterAddress_BMX_SettingsBits = 0xF4;      // 1 byte
const unsigned char AirSensorBMX::RegisterAddress_BMX_ConfigBits = 0xF5;        // 1 byte
const unsigned char AirSensorBMX::RegisterAddress_BMX_Pressure = 0xF7;          // 3 bytes
const unsigned char AirSensorBMX::RegisterAddress_BMX_Temperature = 0xFA;       // 3 bytes
const unsigned char AirSensorBMX::RegisterAddress_BMX_Humidity = 0xFD;          // 2 bytes

const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_T1 = 0x88;   // 2 byte unsigned
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_T2 = 0x8A;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_T3 = 0x8C;   // 2 byte signed

const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P1 = 0x8E;   // 2 byte unsigned
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P2 = 0x90;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P3 = 0x92;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P4 = 0x94;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P5 = 0x96;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P6 = 0x98;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P7 = 0x9A;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P8 = 0x9C;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_P9 = 0x9E;   // 2 byte signed

const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_H1 = 0xA1;   // 1 byte unsigned
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_H2 = 0xE1;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_H3 = 0xE3;   // 1 byte unsigned
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_H4 = 0xE4;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_H5 = 0xE5;   // 2 byte signed
const unsigned char AirSensorBMX::RegisterAddress_BMX_Compensation_H6 = 0xE7;   // 1 byte signed


const unsigned char AirSensorBMX::RegisterValue_BMX_Status_Free = 0x00;
const unsigned char AirSensorBMX::RegisterValue_BMX_StatusBits_Updating = 0x01;
const unsigned char AirSensorBMX::RegisterValue_BMX_StatusBits_Measuring = 0x08;

const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Power_Sleep = 0;
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Power_Forced = 1;
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Power_Normal = 3;

const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Pressure_Disabled = 0 << 2;    // Output set to 0x080000
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Pressure_16bit = 1 << 2;       // 2.62 Pa 
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Pressure_17bit = 2 << 2;       // 1.31 Pa 
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Pressure_18bit = 3 << 2;       // 0.66 Pa
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Pressure_19bit = 4 << 2;       // 0.33 Pa
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Pressure_20bit = 5 << 2;       // 0.16 Pa

const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Temperature_Disabled = 0 << 5; // Output set to 0x080000
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Temperature_16bit = 1 << 5;    // 0.0050 °C 
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Temperature_17bit = 2 << 5;    // 0.0025 °C 
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Temperature_18bit = 3 << 5;    // 0.0012 °C 
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Temperature_19bit = 4 << 5;    // 0.0006 °C 
const unsigned char AirSensorBMX::RegisterValue_BMX_SettingsBits_Temperature_20bit = 5 << 5;    // 0.0003 °C 

const unsigned char AirSensorBMX::RegisterValue_BMX_Reset = 0xB6;

const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_SPI_Disabled = 0;
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_SPI_Enabled = 1;

const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_Filter_Disabled = 0 << 1;    // 1 sample reach >= 75% of step response
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_Filter_X2 = 2 << 1;          // 2 samples reach >= 75% of step response
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_Filter_X4 = 3 << 1;          // 5 samples reach >= 75% of step response
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_Filter_X8 = 4 << 1;          // 11 samples reach >= 75% of step response
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_Filter_X16 = 5 << 1;         // 22 samples reach >= 75% of step response

const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_0ms5 = 0 << 5;
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_62ms5 = 1 << 5;
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_125ms = 2 << 5;
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_250ms = 3 << 5;
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_500ms = 4 << 5;
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_1000ms = 5 << 5;
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_2000ms = 6 << 5;
const unsigned char AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_4000ms = 7 << 5;

const unsigned char AirSensorBMX::RegisterValue_BMX_ChipId_BMP280_Sample1 = 0x56;
const unsigned char AirSensorBMX::RegisterValue_BMX_ChipId_BMP280_Sample2 = 0x57;
const unsigned char AirSensorBMX::RegisterValue_BMX_ChipId_BMP280 = 0x58;
const unsigned char AirSensorBMX::RegisterValue_BMX_ChipId_BME280 = 0x60;
const unsigned char AirSensorBMX::RegisterValue_BMX_ChipId_BME680 = 0x61;
const unsigned char AirSensorBMX::RegisterValue_BMX_ChipId_BME688 = 0x61;

const char* AirSensorBMX::ChipName_BMP280_Sample = "BMP280 Sample";
const char* AirSensorBMX::ChipName_BMP280 = "BMP280";
const char* AirSensorBMX::ChipName_BME280 = "BME280";
const char* AirSensorBMX::ChipName_BME680 = "BME680";
const char* AirSensorBMX::ChipName_BME688 = "BME688";



void AirSensorBMX::SetRegister(unsigned char deviceId, unsigned char registerAddress, unsigned char registerValue)
{
    Board::I2CBeginWrite(this->i2c, deviceId);
    Board::I2CWrite(this->i2c, &registerAddress, 1);
    Board::I2CWrite(this->i2c, &registerValue, 1);
    Board::I2CEndWrite(this->i2c);
}

void AirSensorBMX::GetRegister(unsigned char deviceId, unsigned char registerAddress, void* dest, int size)
{
    Board::I2CBeginWrite(this->i2c, deviceId);
    Board::I2CWrite(this->i2c, &registerAddress, 1);
    Board::I2CEndWrite(this->i2c);

    Board::I2CRequestData(this->i2c, deviceId, size);

    Board::I2CReadLsbFirst(this->i2c, dest, size);
}


void AirSensorBMX::Init(int i2cIndex)
{
    this->i2c = Board::I2CBegin(i2cIndex);
    this->SetSettingsMode(AirSensorBMX::RegisterValue_BMX_SettingsBits_Power_Normal, AirSensorBMX::RegisterValue_BMX_SettingsBits_Pressure_20bit, AirSensorBMX::RegisterValue_BMX_SettingsBits_Temperature_20bit);
    this->SetConfigurationMode(AirSensorBMX::RegisterValue_BMX_ConfigBits_SPI_Disabled, AirSensorBMX::RegisterValue_BMX_ConfigBits_Filter_Disabled, AirSensorBMX::RegisterValue_BMX_ConfigBits_StandbyTime_0ms5);
    this->ReadFeatures();
    this->ReadCompensationParameters();

    this->awaitingData = false;
}

void AirSensorBMX::Reset()
{
    this->SetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Reset, AirSensorBMX::RegisterValue_BMX_Reset);
}

int AirSensorBMX::GetStatus()
{
    uint8_t status;
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Status, &status, 1);
    return status;
}


unsigned char AirSensorBMX::GetChipId()
{
    uint8_t chipId;
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_ChipId, &chipId, 1);

    return (unsigned char)chipId;
}

void AirSensorBMX::SetSettingsMode(unsigned char powerMode, unsigned char pressureMode, unsigned char temperatureMode)
{
    this->SetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_SettingsBits, powerMode | pressureMode | temperatureMode);
}

void AirSensorBMX::SetConfigurationMode(unsigned char spiMode, unsigned char filterMode, unsigned char standbyTime)
{
    this->SetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_ConfigBits, spiMode | filterMode | standbyTime);
}

//
//void AirSensorBMX::ReadData()
//{
//    int32_t pressureRaw = 0;
//    int32_t temperatureRaw = 0;
//    int32_t humidityRaw = 0;
//
//    Board::I2CBeginWrite(this->i2c, AirSensorBMX::DeviceAddress_BMX);
//    Board::I2CWrite(this->i2c, &AirSensorBMX::RegisterAddress_BMX_Temperature, 1);
//    Board::I2CEndWrite(this->i2c);
//
//    Board::I2CRequestData(this->i2c, AirSensorBMX::DeviceAddress_BMX, 3);
//
//    Board::I2CReadMsbFirst(this->i2c, &temperatureRaw, 3);
//
//
//
//
//    Board::I2CBeginWrite(this->i2c, AirSensorBMX::DeviceAddress_BMX);
//    Board::I2CWrite(this->i2c, &AirSensorBMX::RegisterAddress_BMX_Pressure, 1);
//    Board::I2CEndWrite(this->i2c);
//
//    Board::I2CRequestData(this->i2c, AirSensorBMX::DeviceAddress_BMX, 3);
//
//    Board::I2CReadMsbFirst(this->i2c, &pressureRaw, 3);
//
//
//    // Convert from 24 to 20 bits? Bosch sample calculation (from BMP280 doc) seems to assume to convert. But result seems coherent when converting only temperature and not pressure...
//    temperatureRaw >>= 4; 
//    // pressureRaw >>= 4; // Shifting it produces totally unreasonable results, while not shifting it produces much less unreasonable results
//
//
//    // Bosch sample data (from BMP280 datasheet)
//    // pressureRaw = 415148;        // 100653
//    // temperatureRaw = 519888;     // 25.08
//
//
//    int32_t t_fine = this->GetFineTemperatureFactor(temperatureRaw);
//    this->temperature = this->GetTemperatureCompensated(t_fine);
//    this->pressure = this->GetPressureCompensated(pressureRaw, t_fine);
//    this->humidity = this->GetHumidityCompensated(humidityRaw, t_fine);
//
//}

void AirSensorBMX::ReadData()
{
    if (!this->awaitingData)
    {
        Board::I2CBeginWrite(this->i2c, AirSensorBMX::DeviceAddress_BMX);
        Board::I2CWrite(this->i2c, &AirSensorBMX::RegisterAddress_BMX_Pressure, 1); // Set read address offset to Pressure register
        Board::I2CEndWrite(this->i2c);

        Board::I2CRequestData(this->i2c, AirSensorBMX::DeviceAddress_BMX, this->readingBlockSize); // Request data from the read offset onward (P1, P2, P3, T1, T2, T3, H1, H2)
        this->awaitingData = true;
    }

    if (this->awaitingData)
    {
        if (Board::I2CBytesAvailable(this->i2c) >= this->readingBlockSize)
        {
            int32_t pressureRaw = 0;
            int32_t temperatureRaw = 0;
            int32_t humidityRaw = 0;
            Board::I2CReadMsbFirst(this->i2c, &pressureRaw, 3);
            Board::I2CReadMsbFirst(this->i2c, &temperatureRaw, 3);
            if (this->sensesHumidity)
                Board::I2CReadMsbFirst(this->i2c, &humidityRaw, 2);

            // Convert from 24 to 20 bits? Bosch sample calculation (from BMP280 doc) seems to assume to convert. But result seems coherent when converting only temperature and not pressure...
            temperatureRaw >>= 4;
            // pressureRaw >>= 4; // Shifting pressure produces totally unreasonable result, while not shifting it produces much less unreasonable result

            // Bosch sample data (from BMP280 datasheet)
            // pressureRaw = 415148;        // 100653
            // temperatureRaw = 519888;     // 25.08

            int32_t t_fine = this->GetFineTemperatureFactor(temperatureRaw);
            this->temperature = this->GetTemperatureCompensated(t_fine);
            this->pressure = this->GetPressureCompensated(pressureRaw, t_fine);
            this->humidity = this->GetHumidityCompensated(humidityRaw, t_fine);

            this->awaitingData = false;
        }
    }
}


void AirSensorBMX::ReadCompensationParameters()
{
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_T1, &this->dig_T1, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_T2, &this->dig_T2, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_T3, &this->dig_T3, 2);

    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P1, &this->dig_P1, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P2, &this->dig_P2, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P3, &this->dig_P3, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P4, &this->dig_P4, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P5, &this->dig_P5, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P6, &this->dig_P6, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P7, &this->dig_P7, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P8, &this->dig_P8, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_P9, &this->dig_P9, 2);

    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_H1, &this->dig_H1, 1);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_H2, &this->dig_H2, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_H3, &this->dig_H3, 1);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_H4, &this->dig_H4, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_H5, &this->dig_H5, 2);
    this->GetRegister(AirSensorBMX::DeviceAddress_BMX, AirSensorBMX::RegisterAddress_BMX_Compensation_H6, &this->dig_H6, 1);

    // Bosch sample parameters (from BMP280 datasheet)
    // this->dig_T1 = 27504;
    // this->dig_T2 = 26435;
    // this->dig_T3 = -1000;
    // this->dig_P1 = 36477;
    // this->dig_P2 = -10685;
    // this->dig_P3 = 3024;
    // this->dig_P4 = 2855;
    // this->dig_P5 = 140;
    // this->dig_P6 = -7;
    // this->dig_P7 = 15500;
    // this->dig_P8 = -14600;
    // this->dig_P9 = 6000;
}

void AirSensorBMX::ReadFeatures()
{
    this->chipId = this->GetChipId();
    this->sensesTemperature = true;
    this->sensesPressure = true;
    this->sensesHumidity = this->chipId >= AirSensorBMX::RegisterValue_BMX_ChipId_BME280;
    this->sensesGas = this->chipId >= AirSensorBMX::RegisterValue_BMX_ChipId_BME680;
    this->readingBlockSize = this->sensesTemperature * 3 + this->sensesPressure * 3 + this->sensesHumidity * 2;

    switch (chipId)
    {
    case AirSensorBMX::RegisterValue_BMX_ChipId_BMP280_Sample1:
    case AirSensorBMX::RegisterValue_BMX_ChipId_BMP280_Sample2:
        this->chipName = (char*)AirSensorBMX::ChipName_BMP280_Sample;
        break;

    case AirSensorBMX::RegisterValue_BMX_ChipId_BMP280:
        this->chipName = (char*)AirSensorBMX::ChipName_BMP280;
        break;

    case AirSensorBMX::RegisterValue_BMX_ChipId_BME280:
        this->chipName = (char*)AirSensorBMX::ChipName_BME280;
        break;

    case AirSensorBMX::RegisterValue_BMX_ChipId_BME680:
        this->chipName = (char*)AirSensorBMX::ChipName_BME680;
        break;

        //case AirSensorBMX::RegisterValue_BMX_ChipId_BME688:
        //    this->chipName = AirSensorBMX::ChipName_BME688;
    }
}


// Part of Temperature Compensation formula from Bosch BME280 specs
// Returns the "t_fine" as referred to in Bosch specs, which is used for calculating actual temperature, pressure and humidity from raw readings
int32_t AirSensorBMX::GetFineTemperatureFactor(int32_t adc_T)
{
    int32_t x = ((adc_T >> 4) - ((int32_t)this->dig_T1));

    int32_t var1 = ((((adc_T >> 3) - ((int32_t)this->dig_T1 << 1))) * ((int32_t)this->dig_T2)) >> 11;
    int32_t var2 = (((x * x) >> 12) * (int32_t)this->dig_T3) >> 14;

    return  var1 + var2;
}

// Compensation formula from Bosch BME280 specs
// Returns temperature in DegC, resolution is 0.01 DegC. Example value of result “5123” equals 51.23 DegC.
float AirSensorBMX::GetTemperatureCompensated(int32_t t_fine)
{
    int result = (t_fine * 5 + 128) >> 8;

    return (float)result / 100;
}

// Compensation formula from Bosch BME280 specs
// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Example value of p “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
float AirSensorBMX::GetPressureCompensated(int32_t adc_P, int32_t t_fine)
{
    int64_t var1 = ((int64_t)t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)this->dig_P6;

    var2 = var2 + ((var1 * (int64_t)this->dig_P5) << 17);
    var2 = var2 + (((int64_t)this->dig_P4) << 35);

    var1 = ((var1 * var1 * (int64_t)this->dig_P3) >> 8) + ((var1 * (int64_t)this->dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)this->dig_P1) >> 33;

    if (var1 == 0)
        return 0;

    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;

    var1 = (((int64_t)this->dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)this->dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)this->dig_P7) << 4);

    return (float)((uint32_t)p) / 256.0f;
}


// Alternative compensation formula with double precision from Bosch BME280 API on Github
// float BarometerBMP280::GetPressureCompensated(int32_t adc_P, int32_t t_fine)
// {   
//     double var1, var2;
//     double pressure = 0.0;

//     var1 = ((double) t_fine / 2.0) - 64000.0;
//     var2 = var1 * var1 * ((double) this->dig_P6) / 32768.0;
//     var2 = var2 + var1 * ((double) this->dig_P5) * 2.0;
//     var2 = (var2 / 4.0) + (((double) this->dig_P4) * 65536.0);
//     var1 = (((double)this->dig_P3) * var1 * var1 / 524288.0 + ((double)this->dig_P2) * var1) / 524288.0;
//     var1 = (1.0 + var1 / 32768.0) * ((double) this->dig_P1);

//     if (var1 < 0 || var1 > 0)
//     {
//         pressure = 1048576.0 - (double)adc_P;
//         pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
//         var1 = ((double)this->dig_P9) * pressure * pressure / 2147483648.0;
//         var2 = pressure * ((double)this->dig_P8) / 32768.0;

//         pressure = pressure + (var1 + var2 + ((double)this->dig_P7)) / 16.0;
//     }

//     return pressure;
// }


// Compensation formula from Bosch BME280 specs
// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of “47445” represents 47445/1024 = 46.333 %RH
float AirSensorBMX::GetHumidityCompensated(int32_t adc_H, int32_t t_fine)
{
    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)this->dig_H4) << 20) - (((int32_t)this->dig_H5) *
        v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r *
            ((int32_t)this->dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)this->dig_H3)) >> 11) +
                ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)this->dig_H2) +
            8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
        ((int32_t)this->dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    uint32_t h = (uint32_t)(v_x1_u32r >> 12);

    return (float)h / 1024.0f;
}
