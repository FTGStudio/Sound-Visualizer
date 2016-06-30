#include <math.h>
#include <complex.h>
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
				//get_fft(1);
				avgADCValue = GetAvgOfBuf(1);
				Display(avgADCValue, 1);
				//simulate();
				system_state = IDLE;
				break;

			case BUFFER_2_COMPLETE:
				//get_fft(2);
				avgADCValue = GetAvgOfBuf(2);
				Display(avgADCValue, 2);
				//simulate();
				system_state = IDLE;
				break;

			case INITIALIZE:
				Initialize();
				//char str[30];
				//int result = (SysCtlClockGet()/16000)-1;
				//usprintf(str, "Clock Get: %6d \n", result);
				//UARTSend(str, 24);
				system_state = IDLE;
				break;
		}
	}
}
