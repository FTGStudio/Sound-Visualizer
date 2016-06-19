#define BUF_SIZE 3200

void InitializeDisplay();
void InitializeTimers();
void InitializeADC();
void InitializeInterrupts();
unsigned long GetAvgOfBuf(int bufNum);
void Timer1IntHandler();

void svInitializeUart();
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
