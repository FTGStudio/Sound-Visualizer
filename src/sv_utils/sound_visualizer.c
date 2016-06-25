#include <math.h>
//#include <complex.h>

#include "../inc/hw_types.h"
#include "../inc/hw_ints.h"
#include "../inc/hw_memmap.h"
#include "../inc/rit128x96x4.h"

#include "../utils/ustdlib.h"

#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"

//Needed for UART
#include "../driverlib/gpio.h"
#include "../driverlib/uart.h"

//My drivers
#include "../my_driverlib/my_adc.h"
#include "../my_driverlib/my_timer.h"

#include "system_states.h"
#include "sound_visualizer.h"

#define PI 	3.14
#define NUM_BARS 		8
#define LEVEL_HEIGHT	8

unsigned char singleLevelFill[56] = { 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
								  	  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

unsigned char singleLevelClear[56] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned long BufOne[BUF_SIZE];
unsigned long BufTwo[BUF_SIZE];
tBoolean ADCSampleReady = false;
tBoolean BufOneReadyToRead = false;
tBoolean BufTwoReadyToRead = false;
int CurrentWriteBuf = 1;
int BufWriteIndex = 0;
double avg_history_list[8] = {};
int previous_level_list[8] = {};
int current_bar_count = 0;

//Interrupt handler for ADC
void ADCIntHandler()
{
    ADCIntClear(ADC0_BASE, 3);	//Clear the ADC interrupt flag
    ADCSampleReady = true;		//Flag to signal sample is ready
}

// The UART interrupt handler.
void UARTIntHandler(void)
{
    unsigned long ulStatus;
    ulStatus = UARTIntStatus(UART0_BASE, true);	// Get the interrrupt status.
    UARTIntClear(UART0_BASE, ulStatus);	// Clear the asserted interrupts.

    // Loop while there are characters in the receive FIFO.
    while(UARTCharsAvail(UART0_BASE))
    {
    	// Read the next character from the UART and write it back to the UART.
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
    }
}

//Initialize all peripherals
void Initialize()
{
	// Set the clocking 20Mhz (200Mhz/10)
	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);		//Enable the clock to the ADC module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);	//Enable Timer0 peripheral
	InitializeDisplay();
	InitializeTimers();
	InitializeADC();
	svInitializeUart();
	InitializeInterrupts();
}

void InitializeDisplay()
{
	RIT128x96x4Init(1000000);	// Initialize the OLED display.
	//RIT128x96x4StringDraw("Splash Screen!", 30, 40, 15);	//Print Splash Screen
}

void svInitializeUart()
{
	// Enable the peripherals used for UART
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Set GPIO A0 and A1 as UART pins
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Configure the UART for 115,200, 8-N-1 operation
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
	                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	                         UART_CONFIG_PAR_NONE));

}

void InitializeInterrupts()
{
	//Enable and clear all configured interrupts before starting main loop
	IntEnable(INT_ADC3);
	IntEnable(INT_TIMER1A);
	IntMasterEnable();
	ADCIntClear(ADC0_BASE, 3);
}

int GetSystemState()
{
	if(ADCSampleReady)
	{
		ReadSampleToBuf();
		ADCSampleReady = false;
	}
	if(BufOneReadyToRead)
	{
		BufOneReadyToRead = false;
		return BUFFER_1_COMPLETE;
	}
	if(BufTwoReadyToRead)
	{
		BufTwoReadyToRead = false;
		return BUFFER_2_COMPLETE;
	}
	return IDLE;
}

void ReadSampleToBuf()
{
	if(CurrentWriteBuf == 1)
	{
		ADCSequenceDataGet(ADC0_BASE, 3, &BufOne[BufWriteIndex]);
		BufWriteIndex++;
	  	if(BufWriteIndex >= BUF_SIZE)
	   	{
	   		BufWriteIndex = 0;
	   		CurrentWriteBuf = 2;
	   		BufOneReadyToRead = true;
	   	}
	}
	else if(CurrentWriteBuf == 2)
	{
	  	ADCSequenceDataGet(ADC0_BASE, 3, &BufTwo[BufWriteIndex]);
	  	BufWriteIndex++;
	   	if(BufWriteIndex >= BUF_SIZE)
	   	{
	   		BufWriteIndex = 0;
	   		CurrentWriteBuf = 1;
	   		BufTwoReadyToRead = true;
	  	}
	}
}

unsigned long GetBufOneAvg()
{
	unsigned long sum = 0;
	for(int i=0; i<BUF_SIZE; i++)
	{
		sum += BufOne[i];
	}
	return sum / (unsigned long) BUF_SIZE;
}

unsigned long GetBufTwoAvg()
{
	unsigned long sum = 0;
	for(int i=0; i<BUF_SIZE; i++)
	{
		sum += BufTwo[i];
	}
	return sum / (unsigned long) BUF_SIZE;
}


// Send a string to the UART.
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    // Loop while there are more characters to send.
    while(ulCount--)
    {
        // Write the next character to the UART.
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}

void RITAddRow(int difference, int prevDisp, int columnOffset)
{
	int prev_height = LEVEL_HEIGHT * prevDisp;
	int additional_height = difference * LEVEL_HEIGHT;

	for(int row=prev_height; row < prev_height + additional_height; row+=LEVEL_HEIGHT)
	{
		RIT128x96x4ImageDraw(singleLevelFill, (columnOffset * 16) + 1, 96 - row, 14, LEVEL_HEIGHT);
	}
}

void RITClearRow(int difference, int prevDisp, int columnOffset)
{
	int prev_height = LEVEL_HEIGHT * prevDisp;
	int subtract_height = difference * LEVEL_HEIGHT * -1;

	for(int row=prev_height; row > prev_height - subtract_height; row-=LEVEL_HEIGHT)
	{
		RIT128x96x4ImageDraw(singleLevelClear, (columnOffset * 16) + 1, 96 - row, 14, LEVEL_HEIGHT);
	}
}

void simulate()
{
	int disp_level_list[8] = {12, 10, 8, 6, 4, 2, 1, 7};
	int difference_list[8] = {12, -2, -2, -2, -2, -2, -1, 6};
	int prev_level_list[8] = {0, 12, 10, 8, 6, 4, 2, 1};
	static int index = 0;

	if(difference_list[index] < 0)
	{
		RITClearRow(difference_list[index], prev_level_list[index], 0);
		RITClearRow(difference_list[index], prev_level_list[index], 1);
		RITClearRow(difference_list[index], prev_level_list[index], 2);
		RITClearRow(difference_list[index], prev_level_list[index], 3);
		RITClearRow(difference_list[index], prev_level_list[index], 4);
		RITClearRow(difference_list[index], prev_level_list[index], 5);
		RITClearRow(difference_list[index], prev_level_list[index], 6);
		RITClearRow(difference_list[index], prev_level_list[index], 7);
	}
	else
	{
		RITAddRow(difference_list[index], prev_level_list[index], 0);
		RITAddRow(difference_list[index], prev_level_list[index], 1);
		RITAddRow(difference_list[index], prev_level_list[index], 2);
		RITAddRow(difference_list[index], prev_level_list[index], 3);
		RITAddRow(difference_list[index], prev_level_list[index], 4);
		RITAddRow(difference_list[index], prev_level_list[index], 5);
		RITAddRow(difference_list[index], prev_level_list[index], 6);
		RITAddRow(difference_list[index], prev_level_list[index], 7);
	}
	if(index >= 7)
	{
		index = 0;
	}
	else
	{
		index++;
	}
}

void Display(unsigned long avgVal, int bufNum)
{
	static int prevLevel = 0;
	int difference;

	//double new_avg = get_new_average(avgVal);
	//double new_avg = avgVal;
	//avg_history_list[current_bar_count] = avgVal;

	unsigned char uartStr[25];
	usprintf(uartStr, "Avg Val%d: %4u \r", bufNum, avgVal);
	UARTSend(uartStr, 16);

	int dispLevel = (avgVal / 12) + 1;	// Calculates levels 1-12
	if(dispLevel > 12) dispLevel = 12;	//Make sure it doesn't go above 12

	difference = dispLevel - previous_level_list[current_bar_count];

	if(difference > 0)
	{
		RITAddRow(difference, previous_level_list[current_bar_count], current_bar_count);
	}
	else if(difference < 0)
	{
		RITClearRow(difference, previous_level_list[current_bar_count], current_bar_count);
	}

	previous_level_list[current_bar_count] = dispLevel;
	current_bar_count++;
	if(current_bar_count >= NUM_BARS)
	{
		current_bar_count = 0;
		//reset_avg_history();
	}
}

//void _fft(long buf[], long out[], int n, int step)
//{
//	if(step < n)
//	{
//		_fft(out, buf, n, step*2);
//		_fft(out + step, buf + step, n, step *2);
//
//		for(int i=0; i<n; i += 2*step)
//		{
//
//			//long t = cexp(-I);
//			//long t = cexpf(-I * PI * i/n) * out[i + step];
//			long t = cexp(-I * PI * i/n) * out[i + step];
//			buf[i/2] = out[i] + t;
//			buf[(i + n)/2] = out[i] - t;
//		}
//	}
//}
//
//void fft(long buf[], int n)
//{
//	long out[n];
//	for(int i=0; i < n; i++)
//	{
//		out[i] = buf[i];
//	}
//
//	_fft(buf, out, n, 1);
//}
//
//void get_fft(int buffer_number)
//{
//	if(buffer_number == 1)
//	{
//		fft(BufOne, BUF_SIZE);
//	}
//	else if(buffer_number == 2)
//	{
//		fft(BufTwo, BUF_SIZE);
//	}
//}
//
//void compute_average_magnitude(int buffer_number)
//{
//	if(buffer_number == 1)
//	{
//		//for(int i=0)
//	}
//	else if(buffer_number == 2)
//	{
//
//	}
//}
//



double get_new_average(double avg)
{
	double new_avg = avg;
	for(int i=0; i < current_bar_count; i++)
	{
		new_avg += avg_history_list[i];
	}

	new_avg /= (current_bar_count + 1);

	return new_avg;
}

void reset_avg_history()
{
	for(int i=0; i<NUM_BARS; i++)
	{
		avg_history_list[i] = 0;
	}
}
