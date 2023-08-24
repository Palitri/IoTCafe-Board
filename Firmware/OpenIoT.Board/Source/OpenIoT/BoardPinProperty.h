#ifndef BoardPinProperty_h
#define BoardPinProperty_h

#include "Property.h"
#include "Board.h"

class BoardPinProperty :
	public Property
{
private:
	static unsigned char pinModeToPropertyTypeMap[5];
	static unsigned char pinModeToPropertyFlagsMap[5];

public:
	int pin;
	BoardPinMode pinMode;

	BoardPinProperty(void** data);
	BoardPinProperty(int semantic, int pin, BoardPinMode mode);

	virtual void Update();
};

#endif
// BoardPinProperty_h
