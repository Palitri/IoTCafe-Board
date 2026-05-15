#ifndef Thermistor_h
#define Thermistor_h

class Thermistor
{
public:
	// Methods to calculate temperature from resistance, resistance from temperature and Beta coefficient
	// Works with NTC (Negative Temperature Coefficient) thermistors and uses the Beta model for calculations
	// Temperature units must be Kelvin. Resistance units must be Ohm or Ohm multiples (like kOhm)
	static float CalculateTemperature(float resistance, float referenceResistance, float referenceTempKelvin, float betaCoefficient);
	static float CalculateResistance(float tempKelvin, float referenceResistance, float referenceTempKelvin, float betaCoefficient);
	static float CalculateBetaCoefficient(float resistance1, float temp1, float resistance2, float temp2);

	int pin;

	float betaCoefficient, referenceTempKelvin, referenceResistance;

	Thermistor(int pin, float betaCoefficient = 3950.0f, float referenceTempKelvin = 298.15f, float referenceResistance = 100000.0f);
	~Thermistor();

	float ReadTemperature();
};

#endif
// Thermistor_h
