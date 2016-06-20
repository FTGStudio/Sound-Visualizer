#include "inc/hw_adc.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/lm3s6965.h"
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

int main()
{
	int system_state = INITIALIZE_TIMERS;
	// Set the clocking to run directly from the crystal.
//	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
//
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//
//	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//
//	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
//    					(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
//						 UART_CONFIG_PAR_NONE));

//    UARTSend((unsigned char *)"Enter text: ", 12);

	//Main program loop
	unsigned long avgADCValue = 0;
	while(1)
	{
		switch(system_state)
		{
			case IDLE:

				break;

			case BUFFER_1_COMPLETE:

				break;

			case BUFFER_2_COMPLETE:

				break;

			case INITIALIZE_TIMERS:

				// Peripherals include
				InitializeDisplay();
				InitializeTimers();
				InitializeADC();
				svInitializeUart();
				InitializeInterrupts();
				UARTSend((unsigned char *)"Enter text: ", 12);
				system_state = IDLE;
				break;
		}

		svCheckSystemState(&system_state);
	}
}
