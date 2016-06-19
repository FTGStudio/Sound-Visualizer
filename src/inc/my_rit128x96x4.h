/*
 * my_rit128x96x4.h
 *
 *  Created on: Jun 10, 2016
 *      Author: Michael
 */

#ifndef MY_RIT128X96X4_H_
#define MY_RIT128X96X4_H_

//*****************************************************************************
//
// Prototypes for the driver APIs.
//
//*****************************************************************************
extern void RIT128x96x4Init(unsigned long ulFrequency);
extern void RIT128x96x4Clear(void);
extern void RIT128x96x4StringDraw(const char *pcStr,
                                    unsigned long ulX,
                                    unsigned long ulY,
                                    unsigned char ucLevel);
extern void RIT128x96x4ImageDraw(const unsigned char *pucImage,
                                   unsigned long ulX,
                                   unsigned long ulY,
                                   unsigned long ulWidth,
                                   unsigned long ulHeight);



#endif /* MY_RIT128X96X4_H_ */
