//#define BOARD_ESP8266
//#define BOARD_STM32F103C
//#define BOARD_PC

//#define BOARD_PC


//#include "OpenIoT/BoardPC.h"

#include "OpenIoT/OpenIoT.h"

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
