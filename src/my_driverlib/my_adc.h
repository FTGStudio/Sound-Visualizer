/*
 * my_adc.h
 *
 *  Created on: Jun 24, 2016
 *      Author: Michael
 */

#ifndef SRC_MY_DRIVERLIB_MY_ADC_H_
#define SRC_MY_DRIVERLIB_MY_ADC_H_

//*****************************************************************************
//
// Value that can be passed to ADCSequenceConfigure as the ulTrigger
// parameter.
//
//*****************************************************************************
#define ADC_TRIGGER_TIMER       0x00000005  // Timer event

//*****************************************************************************
//
// Values that can be passed to ADCSequenceStepConfigure as the ulConfig
// parameter.
//
//*****************************************************************************
#define ADC_CTL_IE              0x00000040  // Interrupt enable
#define ADC_CTL_END             0x00000020  // Sequence end select
#define ADC_CTL_CH0             0x00000000  // Input channel 0

//****************************************************************************
//
// My functions
//
//****************************************************************************
extern void InitializeADC();

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void ADCIntEnable(unsigned long ulBase, unsigned long ulSequenceNum);
extern void ADCIntClear(unsigned long ulBase, unsigned long ulSequenceNum);
extern void ADCSequenceEnable(unsigned long ulBase, unsigned long ulSequenceNum);
extern void ADCSequenceDisable(unsigned long ulBase, unsigned long ulSequenceNum);
extern void ADCSequenceConfigure(unsigned long ulBase, unsigned long ulSequenceNum,
                                 unsigned long ulTrigger, unsigned long ulPriority);
extern void ADCSequenceStepConfigure(unsigned long ulBase, unsigned long ulSequenceNum,
                                     unsigned long ulStep, unsigned long ulConfig);
extern long ADCSequenceDataGet(unsigned long ulBase, unsigned long ulSequenceNum,
                               unsigned long *pulBuffer);

#endif /* SRC_MY_DRIVERLIB_MY_ADC_H_ */
