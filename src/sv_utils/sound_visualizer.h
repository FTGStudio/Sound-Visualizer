#ifndef __sound_visualizer_h__
#define __sound_visualizer_h__

#define BUF_SIZE 1000	//32 ms /(1/32kHz)

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

#endif
