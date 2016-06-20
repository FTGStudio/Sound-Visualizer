#include "inc/hw_adc.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
//#include "inc/lm3s6965.h"
#include "inc/rit128x96x4.h"

#include "utils/ustdlib.h"

#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "driverlib/uart.h"

#include "sv_utils/system_states.h"
#include "sv_utils/sound_visualizer.h"

void Display(unsigned long avgVal, int bufNum);

int main()
{
	int system_state = INITIALIZE;

	//Main program loop
	unsigned long avgADCValue = 0;
	while(1)
	{
		switch(system_state)
		{
			case IDLE:
				system_state = GetSystemState();
				break;

			case BUFFER_1_COMPLETE:
				avgADCValue = GetAvgOfBuf(1);
				Display(avgADCValue, 1);
				system_state = IDLE;
				break;

			case BUFFER_2_COMPLETE:
				avgADCValue = GetAvgOfBuf(2);
				Display(avgADCValue, 2);
				system_state = IDLE;
				break;

			case INITIALIZE:
				Initialize();
				system_state = IDLE;
				break;
		}
	}
}

void Display(unsigned long avgVal, int bufNum)
{
	unsigned char uartStr[25];
	usprintf(uartStr, "Avg Val%d: %4u \r", bufNum, avgVal);
	UARTSend(uartStr, 16);
}
