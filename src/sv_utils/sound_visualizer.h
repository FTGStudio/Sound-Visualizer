#ifndef __sound_visualizer_h__
#define __sound_visualizer_h__

#define BUF_SIZE 1000	//0.2/(1/32kHz)

//Initialization functions
void Initialize();
void InitializeDisplay();
void svInitializeUart();
void InitializeInterrupts();

void ReadSampleToBuf();
unsigned long GetBufOneAvg();
unsigned long GetBufTwoAvg();
int GetSystemState();
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
void RITAddRow(int difference, int prevDisp, int offset);
void RITClearRow(int difference, int prevDisp, int offset);
void Display(unsigned long avg, int buf_num);
void reset_avg_history();
double get_new_average(double avg);
void simulate();
//void get_fft(int buffer_number);
//void compute_average_magnitude(int buffer_number);




#endif
