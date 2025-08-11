//#define BOARD_ESP8266
//#define BOARD_STM32F103C
//#define BOARD_PC

//#define BOARD_PC


//#include "OpenIoT/BoardPC.h"

#include "OpenIoT/OpenIoT.h"

////////////////////
// For PlatformIO //
////////////////////
void setup() {
	OpenIoTInitialize();
}

void loop() {
	OpenIoTRun();
}

#ifdef BOARD_PC
/////////////////
// For Desktop //
/////////////////
#include <iostream>
int main()
{
	OpenIoTInitialize();

	while (true)
	{
		OpenIoTRun();

		char c = getchar();
		if (c == 13)
			break;
	}

	return 0;
}
#endif
