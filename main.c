#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/lm3s6965.h"

#include "utils/ustdlib.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "my_adc.h"
#include "my_rit128x96x4.h"

#define BUF_SIZE 3200

unsigned long BufOne[BUF_SIZE];
unsigned long BufTwo[BUF_SIZE];
unsigned long BufOneIndex = 0;
unsigned long BufTwoIndex = 0;
int BufOneReadyToRead = 0;
int BufTwoReadyToRead = 0;
int OneSecDelayFlag = 0;
char str[25];

void InitializeDisplay();
void InitializeTimers();
void InitializeADC();
void InitializeInterrupts();
unsigned long GetAvgOfBuf(int bufNum);

//Interupt handler for timer 0. Performs processor trigger to perform ADC
void Timer0IntHandler()
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);	  //Clear the timer interrupt.
    ADCProcessorTrigger(ADC0_BASE, 3);	              //Trigger ADC sequence 3
}

void Timer1IntHandler()
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);	  //Clear the timer interrupt
	OneSecDelayFlag = 1;
}

//Interrupt handler for ADC
void ADCIntHandler()
{
    ADCIntClear(ADC0_BASE, 3);	//Clear the ADC interrupt flag

    if(BufOneIndex < BUF_SIZE)
    {
    	ADCSequenceDataGet(ADC0_BASE, 3, &BufOne[BufOneIndex++]);
    	if(BufOneIndex == BUF_SIZE-1)
    	{
    		BufTwoIndex = 0;
    		BufOneReadyToRead = 1;
    	}
    }
    else if(BufTwoIndex < BUF_SIZE)
    {
    	ADCSequenceDataGet(ADC0_BASE, 3, &BufTwo[BufTwoIndex++]);
    	if(BufTwoIndex == BUF_SIZE-1)
    	{
    		BufOneIndex = 0;
    		BufTwoReadyToRead = 1;
    	}
    }
}

int main()
{
	// Set the clocking to run directly from the crystal.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	//Initialize peripherals
	InitializeDisplay();
	InitializeTimers();
	InitializeADC();
	InitializeInterrupts();

	//Main program loop
	unsigned long avgADCValue = 0;
	while(1)
	{
		if(BufOneReadyToRead)
		{
			avgADCValue = GetAvgOfBuf(1);
			usnprintf(str, 25, "Buf1: %5u", avgADCValue);
			RIT128x96x4StringDraw(str, 0, 0, 15);
			BufOneReadyToRead = 0;
		}
		else if(BufTwoReadyToRead)
		{
			avgADCValue = GetAvgOfBuf(2);
			usnprintf(str, 25, "Buf2: %5u", avgADCValue);
			RIT128x96x4StringDraw(str, 0, 10, 15);
			BufTwoReadyToRead = 0;
		}
	}
}

void InitializeDisplay()
{
	// Initialize the OLED display.
	RIT128x96x4Init(1000000);	//1MHz frequency

	//Print Test String
	RIT128x96x4StringDraw("Splash Screen!", 30, 40, 15);
}

void InitializeADC()
{
	//Enable the clock to the ADC module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);

	//Currently giving error. Can't find SYSCTL_SET0_ADCSPEED_500KSPS
	//Configure the ADC to sample at 500KSps (Kilo Samples per second)
	//SysCtlADCSpeedSet(SYSCTL_SET0_ADCSPEED_500KSPS);	//choices are 125K, 250K, 500K, and 1M

	//Disable Sequence 3 in order to safely configure ADC
	ADCSequenceDisable(ADC0_BASE, 3);

	//Configure Sequence 3: processor trigger, priority=0 (highest priority)
	ADCSequenceConfigure(ADC_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

	//Configure Sequence 3, step 0, analog input 0 | interrupt | end of sequence
	ADCSequenceStepConfigure(ADC_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

	//Enable sequence 3 and it's interrupt
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCIntEnable(ADC0_BASE, 3);
}

void InitializeTimers()
{
	//Enable timer 0 and timer 1 peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	//Disable timer 0 and timer 1 for configuration
	TimerDisable(TIMER0_BASE, TIMER_A);
	TimerDisable(TIMER1_BASE, TIMER_A);

	//Configure timer 0: 32-bit periodic mode for ADC Timer Trigger
	TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_32_BIT_PER);
	//TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);

	//Configure time 0 to generate trigger event for ADC
	//TimerControlTrigger(TIMER0_BASE, TIMER_BOTH, true);

	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/16000);	//16MHz interrupt
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet());	    //1 second interrupt

	//Enable timer and its interrupt
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);
	TimerEnable(TIMER1_BASE, TIMER_A);
}

void InitializeInterrupts()
{
	//Enable and clear all configured interrupts before starting main loop
	IntEnable(INT_ADC3);
	IntEnable(INT_TIMER0A);
	IntEnable(INT_TIMER1A);
	IntMasterEnable();
	ADCIntClear(ADC0_BASE, 3);
}

unsigned long GetAvgOfBuf(int bufNum)
{
	int i = 0;
	unsigned long sum = 0;
	if(bufNum == 1)
	{
		for(i=0; i<BUF_SIZE; i++)
		{
			sum += BufOne[i];
		}
	}
	else
	{
		for(i=0; i<BUF_SIZE; i++)
		{
			sum += BufTwo[i];
		}
	}
	return sum / BUF_SIZE;
}
