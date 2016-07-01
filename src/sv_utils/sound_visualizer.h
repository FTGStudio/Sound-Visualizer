#ifndef __sound_visualizer_h__
#define __sound_visualizer_h__

#define BUF_SIZE 1000	//0.2/(1/32kHz)

//Initialization functions
extern void Initialize();
extern void InitializeDisplay();


extern void ReadSampleToBuf();
extern unsigned long GetBufOneAvg();
extern unsigned long GetBufTwoAvg();
extern int GetSystemState();

extern void RITAddBarLevels(int difference, int prevLevel, int barIndex);
extern void RITClearBarLevels(int difference, int prevLevel, int barIndex);
extern void Display(unsigned long avg);

//void svInitializeUart();
//void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
//void reset_avg_history();
//double get_new_average(double avg);
//void simulate();
//void get_fft(int buffer_number);
//void compute_average_magnitude(int buffer_number);




#endif
