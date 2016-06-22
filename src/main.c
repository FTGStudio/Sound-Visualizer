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
				//char str[30];
				//int result = (SysCtlClockGet()/16000)-1;
				//usprintf(str, "Clock Get: %6d \n", result);
				//UARTSend(str, 24);
				system_state = IDLE;
				break;
		}
	}
}

void Display(unsigned long avgVal, int bufNum)
{
	static int prevLevel = 0;
	int difference;
	unsigned char uartStr[25];
	usprintf(uartStr, "Avg Val%d: %4u \r", bufNum, avgVal);
	UARTSend(uartStr, 16);

	int dispLevel = (avgVal / 10) + 1;// Calculates levels 1-8
//	unsigned char singleLine[64] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//		 	 	 	 	 	 	     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//								     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//								     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//								     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//								     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//								     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//								     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	// Use previous average value to determine how many rows to clear or add
	difference = dispLevel - prevLevel;
	if(difference > 0)
	{
		RITAddRow(difference, prevLevel);
	}
	else if(difference < 0)
	{
		RITClearRow(difference, prevLevel);
	}
	prevLevel = dispLevel;
//	RIT128x96x4Clear();
//	for(int row=0; row < (dispLevel * 12); row++)
//	{
//		RIT128x96x4ImageDraw(singleLine, 0, row, 128, 1);
//	}

}
