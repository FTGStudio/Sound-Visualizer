extern void my_RIT128x96x4Init(unsigned long ulFrequency);
extern void RIT128x96x4ImageDraw(const unsigned char *pucImage, unsigned long ulX,
                     unsigned long ulY, unsigned long ulWidth,
                     unsigned long ulHeight);
extern void RIT128x96x4Enable(unsigned long ulFrequency);
static void RITWriteCommand(const unsigned char *pucBuffer, unsigned long ulCount);
static void RITWriteData(const unsigned char *pucBuffer, unsigned long ulCount);
