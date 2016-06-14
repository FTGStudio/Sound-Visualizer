/*
 * my_adc.h
 *
 *  Created on: Jun 10, 2016
 *      Author: Michael
 */

#ifndef MY_ADC_H_
#define MY_ADC_H_

//*****************************************************************************
//
// Values that can be passed to ADCSequenceConfigure as the ulTrigger
// parameter.
//
//*****************************************************************************
#define ADC_TRIGGER_PROCESSOR   0x00000000  // Processor event

//*****************************************************************************
//
// Values that can be passed to ADCSequenceStepConfigure as the ulConfig
// parameter.
//
//*****************************************************************************
#define ADC_CTL_CH0             0x00000000  // Input channel 0
#define ADC_CTL_IE              0x00000040  // Interrupt enable
#define ADC_CTL_END             0x00000020  // Sequence end select


//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void ADCSequenceEnable(unsigned long ulBase, unsigned long ulSequenceNum);
extern void ADCSequenceDisable(unsigned long ulBase, unsigned long ulSequenceNum);

extern void ADCSequenceConfigure(unsigned long ulBase, unsigned long ulSequenceNum,
						unsigned long ulTrigger, unsigned long ulPriority);

extern void ADCSequenceStepConfigure(unsigned long ulBase, unsigned long ulSequenceNum,
                         unsigned long ulStep, unsigned long ulConfig);

extern long ADCSequenceDataGet(unsigned long ulBase, unsigned long ulSequenceNum,
                         unsigned long *pulBuffer);

extern void ADCIntEnable(unsigned long ulBase, unsigned long ulSequenceNum);
extern void ADCIntDisable(unsigned long ulBase, unsigned long ulSequenceNum);
extern void ADCIntClear(unsigned long ulBase, unsigned long ulSequenceNum);
extern void ADCProcessorTrigger(unsigned long ulBase, unsigned long ulSequenceNum);


#endif /* MY_ADC_H_ */
