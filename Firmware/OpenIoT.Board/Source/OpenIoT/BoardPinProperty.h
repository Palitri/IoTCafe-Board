#ifndef BoardPinProperty_h
#define BoardPinProperty_h

#include "Property.h"
#include "Board.h"

class BoardPinProperty :
	public Property
{
private:
	static unsigned char pinModeToPropertyTypeMap[6];
	static unsigned char pinModeToPropertyFlagsMap[6];

public:
	int pin;
	BoardPinMode pinMode;

	BoardPinProperty(void** data);
	BoardPinProperty(int semantic, int pin, BoardPinMode mode);

	virtual void Update();
};

#endif
// BoardPinProperty_h
