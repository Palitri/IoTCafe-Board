#ifndef ServoControl_h
#define ServoControl_h

class ServoControl
{
private:
	void* servo;

public:
	int pin;

	ServoControl(int pin);

	void WriteMicroseconds(int microseconds);
	void WriteDegreece(float degreece);
};

#endif
// ServoControl_h