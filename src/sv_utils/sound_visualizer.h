#ifndef __sound_visualizer_h__
#define __sound_visualizer_h__

#define BUF_SIZE 6400	//0.2/(1/32kHz)

//Initialization functions
void Initialize();
void InitializeDisplay();
void InitializeTimers();
void InitializeADC();
void svInitializeUart();
void InitializeInterrupts();

void ReadSampleToBuf();
unsigned long GetAvgOfBuf(int bufNum);
int GetSystemState();
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
void RITAddRow(int difference, int prevDisp, int offset);
void RITClearRow(int difference, int prevDisp, int offset);

#endif
