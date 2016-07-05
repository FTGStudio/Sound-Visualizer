#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

//"My" driver libraries
#include "../my_driverlib/my_adc.h"
#include "../my_driverlib/my_timer.h"
#include "../my_driverlib/my_rit.h"

#include "system_states.h"
#include "sound_visualizer.h"

#define NUM_BARS 		8
#define LEVEL_HEIGHT	8

//Enough pixel values to fill 1 bar's level
unsigned char singleLevelFill[56] = { 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
								  	  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
									  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

//Enough pixel values to clear 1 bar's level
unsigned char singleLevelClear[56] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

//The two buffers for storing ADC values in
unsigned long BufOne[BUF_SIZE];
unsigned long BufTwo[BUF_SIZE];

//Flags to signal that one of the buffers is ready to read/display
tBoolean BufOneReadyToRead = false;
tBoolean BufTwoReadyToRead = false;

tBoolean ADCSampleReady = false;	//Flag to signal that a sample is ready in the ADC FIFO

int CurrentWriteBuf = 1;	//The current buffer to write to (either a 1 or a 2)
int BufWriteIndex = 0;		//The current index within the buffer to write to

int current_bar_index = 0;	//Current bar to update
int previous_level_list[8] = {0,0,0,0,0,0,0,0};	//List of the previous display level for each of the 8 bars

/*
 * Description: Interrupt routine to service the ADC interrupt. It simply
 * clears the interrupt in the register and sets a flag signaling that a sample
 * is ready to be read from the FIFO
 */
void ADCIntHandler()
{
    ADCIntClear(ADC0_BASE, 3);	//Clear the ADC interrupt flag
    ADCSampleReady = true;		//Flag to signal sample is ready
}

/*
 * Description: Initializes the system peripherals, along with the
 * system clock and interrupts.
 */
void Initialize()
{
	// Set the clocking 20Mhz (200Mhz/10)
	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);		//Enable the clock to the ADC module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);	//Enable Timer0 peripheral
	InitializeDisplay();
	InitializeTimers();
	InitializeADC();
	IntMasterEnable();
	ADCIntClear(ADC0_BASE, 3);
}


/*
 * Description: Initialize the OLED display to enable visualizing the sound.
 */
void InitializeDisplay()
{
	// Initialize the OLED display.
	RIT128x96x4Init(1000000);
}


/*
 * Description: It first checks to see if an ADC sample is ready to be read.
 * If it is, it reads it calls a function to read the value into a buffer. It
 * then checks if either of the two buffers is full. If it is, it will change the
 * state of the state machine to display that buffer.
 *
 * Returns: the state of the system for the state machine
 */
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

/*
 * Description: When a sample is ready to be read, this function is called to
 * read that value into the current write buffer. If it fills up the buffer,
 * then a flag is set to notify the program that it is ready to be read and
 * displayed. It also sets the current write buffer to the other buffer, so
 * that buffer will begin to get filled.
 */
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

/*
 * Description: Finds the average of the first buffer by adding all the values
 * together and dividing by the buffer size.
 *
 * Returns: the calculated average
 */
unsigned long GetBufOneAvg()
{
	unsigned long sum = 0;
	for(int i=0; i<BUF_SIZE; i++)
	{
		sum += BufOne[i];
	}
	return sum / (unsigned long) BUF_SIZE;
}

/*
 * Description: Finds the average of the second buffer by adding all the values
 * together and dividing by the buffer size.
 *
 * Returns: the calculated average
 */
unsigned long GetBufTwoAvg()
{
	unsigned long sum = 0;
	for(int i=0; i<BUF_SIZE; i++)
	{
		sum += BufTwo[i];
	}
	return sum / (unsigned long) BUF_SIZE;
}

/*
 * Description: Calculates the bar's level height based on the average value.
 * If the height remains the same, nothing is done. If the calculated height is
 * higher than the previous height, the necessary number of levels are added. If
 * the calculated height is less than the previous height, the necessary number of
 * levels are cleared. (This was found to create a less "flashy" display instead of
 * clearing the whole screen each time).
 *
 * Params: The average value calculated from a buffer
 * Returns: Nothing
 */
void Display(unsigned long avgVal)
{
	int dispLevel = (avgVal / 12) + 1;	// Calculates levels 1-12
	if(dispLevel > 12) dispLevel = 12;	//Make sure it doesn't go above 12

	//Calculate difference in level from its previous state
	int difference = dispLevel - previous_level_list[current_bar_index];

	if(difference > 0)
	{
		//New level is higher than previous, so add levels
		RITAddBarLevels(difference, previous_level_list[current_bar_index], current_bar_index);
	}
	else if(difference < 0)
	{
		//New level is lower than previous, so subtract levels
		RITClearBarLevels(difference, previous_level_list[current_bar_index], current_bar_index);
	}

	//Set new level as the previous level for next update
	previous_level_list[current_bar_index] = dispLevel;

	current_bar_index++;	//Move to next bar
	if(current_bar_index >= NUM_BARS)
	{
		//Roll over back to 0 after the 8th bar
		current_bar_index = 0;
	}
}

/*
 * Description: This calculated the number of levels that need to be added for
 * a specific bar and draws them out on the display.
 *
 * Params: difference-The difference in levels between the new and previous bar levels
 * 		   prevDisp-The previous level for that bar
 * 		   barIndex-The bar that is being added to
 */
void RITAddBarLevels(int difference, int prevLevel, int barIndex)
{
	int prev_height = LEVEL_HEIGHT * prevLevel;	//Calculate the previous height of the bar (in pixels)
	int additional_height = difference * LEVEL_HEIGHT;	//Calculate the height needed to be added (in pixels)

	//Loop over image draw function to draw one level at a time
	for(int row=prev_height; row < prev_height + additional_height; row+=LEVEL_HEIGHT)
	{
		RIT128x96x4ImageDraw(singleLevelFill, (barIndex * 16) + 1, 96 - row, 14, LEVEL_HEIGHT);
	}
}

/*
 * Description: This calculated the number of levels that need to be cleared for
 * a specific bar and clears them out on the display.
 *
 * Params: difference-The difference in levels between the new and previous bar levels
 * 		   prevDisp-The previous level for that bar
 * 		   barIndex-The bar that is being subtracted from
 */
void RITClearBarLevels(int difference, int prevLevel, int barIndex)
{
	int prev_height = LEVEL_HEIGHT * prevLevel;	//Calculate the previous height of the bar (in pixels)
	int subtract_height = difference * LEVEL_HEIGHT * -1; //Calculate the height needed to be subtracted (in pixels)

	//Loop over image draw function to draw(clear) one level at a time
	for(int row=prev_height; row > prev_height - subtract_height; row-=LEVEL_HEIGHT)
	{
		RIT128x96x4ImageDraw(singleLevelClear, (barIndex * 16) + 1, 96 - row, 14, LEVEL_HEIGHT);
	}
}
