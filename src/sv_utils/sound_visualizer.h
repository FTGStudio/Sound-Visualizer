#define BUF_SIZE 3200	//0.2/(1/16kHz)

//Initialization functions
void Initialize();
void InitializeDisplay();
void InitializeTimers();
void InitializeADC();
void svInitializeUart();
void InitializeInterrupts();

<<<<<<< HEAD
void svInitializeUart();
void UARTSend();
void svCheckSystemState(int *systemState);
=======
void ReadSampleToBuf();
unsigned long GetAvgOfBuf(int bufNum);
int GetSystemState();
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
>>>>>>> origin/Mike_D
