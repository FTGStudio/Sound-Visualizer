#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"
#include "my_adc.h"
#include "my_rit128x96x4.h"
//#include "inc/lm3s6965.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

void InitializeDisplay();
void InitializeADC();
void InitializeInterrupts();

//Interrupt handler
void ADCIntHandler()
{
    //Clear the ADC interrupt flag
    ADCIntClear(ADC0_BASE, 1);

    unsigned long adcResult;	//Create buffer for this instead
    ADCSequenceDataGet(ADC0_BASE, 1, &adcResult);
}

int main()
{
	// Set the clocking to run directly from the crystal.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	InitializeDisplay();
	InitializeADC();

	//Main program loop
	while(1)
	{

	}

}

void InitializeDisplay()
{
	// Initialize the OLED display.
	RIT128x96x4Init(1000000);	//1MHz frequency

	//Print Test String
	RIT128x96x4StringDraw("Splash Screen!", 20, 40, 15);
}

void InitializeADC()
{
	//Enable the ADC on channel 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	//Disable Sequence 1 in order to configure ADC
	ADCSequenceDisable(ADC0_BASE, 1);

	ADCSequenceConfigure(ADC_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC_BASE, 1, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

	ADCSequenceEnable(ADC0_BASE, 1);	//Enable sequence 1
	ADCIntEnable(ADC0_BASE, 1);
	IntEnable(INT_ADC1);	//Enable interrupt for ADC Sequence Step 1
}

void InitializeInterrupts()
{
	//Enable and clear all interrupts before starting main loop
	IntMasterEnable();
	ADCIntClear(ADC0_BASE, 1);
}
